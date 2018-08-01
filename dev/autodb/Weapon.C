#include "Weapon.H"
#include "Log.H"
#include "Exception.H"
#include "Item.H"
#include "Cmd.H"
#include "CmdBuffer.H"
#include "Vars.H"
#include "Switches.H"
#include "Actor.H"
#include <data.h>
#include <unordered_map>
#include <vector>
#include <cctype>

struct ActorInfo {
    int num_actors = 0;
    int first_not_ayen_id = 0;
};

static ActorInfo getActorInfo() {

    ActorInfo act_info;

    act_info.num_actors = Data::actors.size();

    //Cut off placeholders for now
    act_info.num_actors = std::min(act_info.num_actors, 40);
    logInf("Found (", act_info.num_actors, ") actors!");

    for(auto& act: Data::actors) {
        if (act.name.substr(0, 4) != "Ayen") {
            logInf("First Actor after the Ayens is ", act);
            act_info.first_not_ayen_id = act.ID;
            break;
        }
    }

    if(act_info.first_not_ayen_id == 0) {
        die("Could not find actors that are not Ayen!");
    }

    return act_info;
}

//Ayens animation id -> [ actor : actors_anim_id ]
using AnimationMap = std::unordered_map<int, std::vector<int>>;

static AnimationMap makeWeaponAnimationMap(const ActorInfo& act_info) {
    logInf("Collecting Weapon Animations...");

    AnimationMap amap;


    for(int widx = kFirstWeapon - 1; widx < kLastWeapon; ++widx) {
        auto& item = Data::items[widx];
        if(item.type != RPG::Item::Type_weapon) {
            continue;
        }

        logDbg("Checking Weapon ", item, " ...");

        auto anim_idx = item.animation_id - 1;

        auto& anim = Data::animations[anim_idx];
        if(anim.name.substr(0, 5) != "Ayen_") {
            die("Weapon ", item, " has bad animation ", anim);
            continue;
        }

        auto rc = amap.insert({item.animation_id, {}});
        if(rc.second) {
            logInf("Found Ayen Weapon Animation ", anim);
            rc.first->second.resize(act_info.num_actors, 0);
        }
    }

    logInf("Generating actor name prefixes...");

    auto actor_name_prefix = std::vector<std::string>();
    for (auto& act: Data::actors) {
        actor_name_prefix.push_back(act.name.substr(0, 4));
    }

    logInf("Creating animation name to id...");
    std::unordered_map<std::string,int> anim_name_id;
    for (auto& anim: Data::animations) {
        //All valid weapon animations have at least one _ in the name.
        if(anim.name.find('_') == std::string::npos) {
            continue;
        }
        auto rc = anim_name_id.insert({anim.name, anim.ID});
        if (!rc.second) {
            auto oldid = rc.first->second;
            auto& oldanim = Data::animations[oldid - 1];
            die("Multiple animations with same name! ", oldanim, " and ", anim);
        }
    }

    logInf("Mapping Ayen's weapon animations to other chars...");
    for (int act_idx = act_info.first_not_ayen_id - 1; act_idx < act_info.num_actors; ++act_idx) {
        auto& act = Data::actors[act_idx];
        auto& prefix = actor_name_prefix[act_idx];
        for (auto& ap: amap) {
            auto anim_idx = ap.first - 1;
            auto& other_actor_anims = ap.second;
            auto& anim = Data::animations[anim_idx];
            //Remove "Ayen" from "Ayen_XXXXXX"
            auto suffix = anim.name.substr(4);
            auto new_name = prefix + suffix;

            auto iter = anim_name_id.find(new_name);
            if(iter == anim_name_id.end()) {
                logDbg("Could not find animation with name `", new_name, "'");
                continue;
            }

            auto mapped_anim = Data::animations[iter->second - 1];

            logInf("Mapped Animation for ", act, " : ", anim, " -> ", mapped_anim);
            other_actor_anims[act_idx] = iter->second;
        }
    }

    return amap;
}

struct WeaponMappingRange {
    int first_orig_id = 0;
    int last_orig_id = 0;
    int first_mapped_id = 0;
    int last_mapped_id = 0;
    bool has_alts = false;
};

using ActorsWeaponMap = std::vector<std::vector<WeaponMappingRange>>;

static ActorsWeaponMap createWeaponCopies(const ActorInfo& act_info, const AnimationMap& amap) {
    logInf("Creating weapon copies...");

    auto actors_wmap = ActorsWeaponMap(act_info.num_actors);

    constexpr auto marker_name = "__WEAPON_ANIMS__";

    int copy_idx = -1;
    int num_items = Data::items.size();
    for (auto& item: Data::items) {
        if(item.name == marker_name) {
            //We start *AFTER* the marker, so we don't do usual -1 to compute the idx here..
            copy_idx = item.ID;
            break;
        }
    }

    if (copy_idx == -1) {
        die("Could not find starting point for weapon copies! : Can't find item called `", marker_name, "'");
    }

    for (auto actor_id = act_info.first_not_ayen_id; actor_id <= act_info.num_actors; ) {
        if (copy_idx >= num_items) {
            die("Not enough items for the weapon copies! copy_idx = ", copy_idx);
        }

        const auto actor_idx = actor_id - 1;
        auto& actor = Data::actors[actor_idx];
        auto& wmap = actors_wmap[actor_idx];

        auto name_prefix = actor.name.substr(0, 4);
        for(auto& c: name_prefix) {
            c = std::toupper(c);
        }

        bool is_fleet = (actor.name == "Fleet");
        bool is_herp = (!is_fleet && actor.name == "Herp");

        int prev_weap_id = 0;

        for(auto item_id = kFirstWeapon; item_id <= kLastWeapon; ++item_id) {
            const auto item_idx = item_id -1;
            const auto& orig_item = Data::items[item_idx];

            if(orig_item.type != RPG::Item::Type_weapon) {
                continue;
            }

            auto orig_anim_idx = orig_item.animation_id - 1;
            auto& orig_anim = Data::animations[orig_anim_idx];

            auto actor_can_use = orig_item.actor_set[actor_idx];
            if(!actor_can_use) {
                continue;
            }

            auto item_copy = orig_item;

            //Fix ID
            item_copy.ID = copy_idx + 1;

            //Only this char can use the copy.
            for(auto&& b: item_copy.actor_set) {
                b = false;
            }
            item_copy.actor_set[actor_idx] = true;

            //Price is 0
            item_copy.price = 0;

            //Remove $A, add name prefix to name. Truncate to max RM2K name length.
            item_copy.name = (name_prefix + item_copy.name.substr(2)).substr(0, kMaxItemNameLen);

            //Fix Description
            item_copy.description = (actor.name + " " + orig_item.name + " ANIMATION COPY");

            //Change Animation
            auto iter = amap.find(orig_item.animation_id);
            if (iter == amap.end()) {
                die("Could not find animation ", orig_anim, " in the amap for ", orig_item);
            }

            auto new_anim_id = iter->second[actor_idx];
            if (new_anim_id == 0) {
                die("Actor ", actor, " has no weapon animation mapped to ", orig_anim,
                        " which is required for item ", orig_item);
            }

            item_copy.animation_id = new_anim_id;

            Data::items[copy_idx] = item_copy;
            ++copy_idx;

            logInf("Created item copy for ", actor, " : ", orig_item, " -> ", item_copy);

            if(prev_weap_id != orig_item.ID - 1) {
                //Make new range if not contiguous
                WeaponMappingRange wm;
                wm.first_orig_id = orig_item.ID;
                wm.first_mapped_id = item_copy.ID;
                wmap.push_back(wm);
            }
            auto& wm = wmap.back();
            wm.last_orig_id = orig_item.ID;
            wm.last_mapped_id = item_copy.ID;

            prev_weap_id = orig_item.ID;

            if (is_fleet) {
                //Add second copy for Eagle Eye
                item_copy.ID = copy_idx + 1;

                item_copy.name = item_copy.name.substr(0, kMaxItemNameLen-2) + "EE";
                item_copy.description += "(Eagle Eye)";

                //99% accuracy for eagle eye guns.
                item_copy.hit = 99;

                Data::items[copy_idx] = item_copy;
                ++copy_idx;

                logInf("Created item copy for ", actor, " (Eagle Eye) : ", orig_item, " -> ", item_copy);

                wm.last_mapped_id = item_copy.ID;
            }

            if(is_herp) {
                //Add second copy for special animation.
                item_copy.ID = copy_idx + 1;

                item_copy.name = item_copy.name.substr(0, kMaxItemNameLen-1) + "2";
                item_copy.description += "(Special Anim)";

                //FIXME: Hardcoded to next animation??
                item_copy.animation_id++;

                Data::items[copy_idx] = item_copy;
                ++copy_idx;

                logInf("Created item copy for ", actor, " (Special Anim) : ", orig_item, " -> ", item_copy);

                wm.last_mapped_id = item_copy.ID;
            }
        }

        ++actor_id;
    }

    return actors_wmap;
}

static void genWeaponSwitch(int act_id,
        const std::vector<WeaponMappingRange>& wmap,
        CmdBuffer& wr_cmds,
        CmdBuffer& ws_cmds,
        bool is_main) {

    const auto act_idx = act_id - 1;
    const auto& actor = Data::actors[act_idx];

    const bool is_fleet = (act_id == eACTFleet);
    const bool is_herp = (act_id == eACTHerp);

    const auto var_id = is_main ? eVarMenuLocal1 : eVarMenuLocal2;

    ws_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                Cmd::ConditionalBranch::eCmpNE, 0, false));
    wr_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                Cmd::ConditionalBranch::eCmpNE, 0, false));

    auto eslot = (is_main ? Cmd::ChangeEquipment::eWeapon : Cmd::ChangeEquipment::eShield);

    ws_cmds.push_back(Cmd::ChangeEquipment::Fixed::Remove(act_id, eslot));
    ws_cmds.push_back(Cmd::ChangeItems::Decrease::Variable::Constant(var_id, 1));

    wr_cmds.push_back(Cmd::ChangeEquipment::Fixed::Remove(act_id, eslot));
    wr_cmds.push_back(Cmd::ChangeItems::Decrease::Variable::Constant(var_id, 1));

    for(auto& wr: wmap) {
        logInf("Actor ", actor, " Mapping weapon range [",
                wr.first_orig_id, ", ", wr.last_orig_id, "] -> [",
                wr.first_mapped_id, ", ", wr.last_mapped_id, "]");

        auto orig_size = wr.last_orig_id - wr.first_orig_id + 1;
        auto mapped_size = wr.last_mapped_id - wr.first_mapped_id + 1;

        if (mapped_size < orig_size) {
            die("Weapon Range mapped size  < orig_size!");
        }

        auto multi = mapped_size / orig_size;
        auto multi_mod = mapped_size % orig_size;

        if (multi_mod != 0) {
            die("Weapon Range mapped size is not an exact multiple of orig size!");
        }

        RPG::EventCommand comment;
        if(wr.first_orig_id == wr.last_orig_id) {
            ws_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                        Cmd::ConditionalBranch::eCmpEQ, wr.first_orig_id, false));
            comment = Cmd::Comment("[" + Data::items[wr.first_orig_id-1].name + "]");
        } else {
            ws_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                        Cmd::ConditionalBranch::eCmpGE, wr.first_orig_id, false));
            ws_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                        Cmd::ConditionalBranch::eCmpLE, wr.last_orig_id, false));

            comment = Cmd::Comment("[" + Data::items[wr.first_orig_id-1].name
                    + " - " + Data::items[wr.last_orig_id-1].name + "]");
        }

        if(wr.first_mapped_id == wr.last_mapped_id) {
            wr_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                        Cmd::ConditionalBranch::eCmpEQ, wr.first_mapped_id, false));
        } else {
            wr_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                        Cmd::ConditionalBranch::eCmpGE, wr.first_mapped_id, false));
            wr_cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(var_id,
                        Cmd::ConditionalBranch::eCmpLE, wr.last_mapped_id, false));
        }

        ws_cmds.push_back(comment);
        wr_cmds.push_back(comment);

        if (multi == 1) {
            //Simple case 1 to 1 mapping
            auto delta = wr.first_mapped_id - wr.first_orig_id;

            ws_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eAdd, delta));
            wr_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eSub, delta));
        } else {
            ws_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eSub, wr.first_orig_id));
            ws_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eMul, multi));
            ws_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eAdd, wr.first_mapped_id));


            if (is_fleet) {
                ws_cmds.push_back(Cmd::Comment("Fleet: Select weapon variant for Eagle Eye"));
                ws_cmds.push_back(Cmd::ConditionalBranch::Switch(eSwFleetEagleEye, true, false));
                ws_cmds.push_back(Cmd::ControlVars::Single::Constant(eVarMenuLocal1, Cmd::ControlVars::eAdd, 1));
                ws_cmds.push_back(Cmd::END());
                ws_cmds.push_back(Cmd::EndBranch());
            }

            if (is_herp && !is_main) {
                ws_cmds.push_back(Cmd::Comment("Herp: Special animation for off hand"));
                ws_cmds.push_back(Cmd::ControlVars::Single::Constant(eVarMenuLocal1, Cmd::ControlVars::eAdd, 1));
            }

            //We only adjust WEAPON_SET. The division in WEAPON_RESET always works to restore original
            //weapon regardless of which variant is active. This is great as we can debug the game
            //and manually hack abilities on and off without worrying about syncing with the weapon
            //set/reset events.

            wr_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eSub, wr.first_mapped_id));
            wr_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eDiv, multi));
            wr_cmds.push_back(Cmd::ControlVars::Single::Constant(var_id, Cmd::ControlVars::eAdd, wr.first_orig_id));

        }

        ws_cmds.push_back(Cmd::END());
        ws_cmds.push_back(Cmd::EndBranch());
        if(wr.first_orig_id != wr.last_orig_id) {
            ws_cmds.push_back(Cmd::END());
            ws_cmds.push_back(Cmd::EndBranch());
        }

        wr_cmds.push_back(Cmd::END());
        wr_cmds.push_back(Cmd::EndBranch());
        if(wr.first_mapped_id != wr.last_mapped_id) {
            wr_cmds.push_back(Cmd::END());
            wr_cmds.push_back(Cmd::EndBranch());
        }
    }


    wr_cmds.push_back(Cmd::ChangeItems::Increase::Variable::Constant(var_id, 1));
    wr_cmds.push_back(Cmd::ChangeEquipment::Fixed::Change::Variable(act_id, var_id));

    ws_cmds.push_back(Cmd::ChangeItems::Increase::Variable::Constant(var_id, 1));
    ws_cmds.push_back(Cmd::ChangeEquipment::Fixed::Change::Variable(act_id, var_id));

    if (!is_main) {
        //When char only has weapon in off hand, the last equip put it in the main hand.
        //We need to equip again into the off hand, then remove the main.

        auto fn = [&](auto& cmds) {
            cmds.push_back(Cmd::ConditionalBranch::Variable::Constant(eVarMenuLocal1,
                        Cmd::ConditionalBranch::eCmpEQ, 0, false));
            cmds.push_back(Cmd::Comment("Special case when main hand was empty but off-hand not"));
            cmds.push_back(Cmd::Comment_2("Put another into offhand, then remove main."));

            //Put another in off-hand
            cmds.push_back(Cmd::ChangeItems::Increase::Variable::Constant(var_id, 1));
            cmds.push_back(Cmd::ChangeEquipment::Fixed::Change::Variable(act_id, var_id));

            //Remove from main
            cmds.push_back(Cmd::ChangeEquipment::Fixed::Remove(act_id, Cmd::ChangeEquipment::eWeapon));
            cmds.push_back(Cmd::ChangeItems::Decrease::Variable::Constant(var_id, 1));

            cmds.push_back(Cmd::END());
            cmds.push_back(Cmd::EndBranch());
        };

        fn(wr_cmds);
        fn(ws_cmds);
    }

    wr_cmds.push_back(Cmd::END());
    wr_cmds.push_back(Cmd::EndBranch());

    ws_cmds.push_back(Cmd::END());
    ws_cmds.push_back(Cmd::EndBranch());
}

static void regenCommonEvents(const ActorInfo& act_info, const ActorsWeaponMap& actors_wmap) {
    logInf("Regenerating Weapon Set/Reset common events ...");
    int weap_reset_id = 0;
    int weap_set_id = 0;

    constexpr auto weap_reset_name = "WEAPON_RESET";
    constexpr auto weap_set_name = "WEAPON_SET";

    for (auto& ce: Data::commonevents) {
        if (ce.name == weap_reset_name) {
            if (weap_reset_id != 0) {
                die("Multiple common events named ", weap_reset_name, " (", weap_reset_id, ") and (", ce.ID);
            }
            weap_reset_id = ce.ID;
        }
        if (ce.name == weap_set_name) {
            if (weap_set_id != 0) {
                die("Multiple common events named ", weap_set_name, " (", weap_set_id, ") and (", ce.ID);
            }
            weap_set_id = ce.ID;
        }
    }

    if(weap_reset_id == 0) {
        die("Could not find common event named", weap_reset_name);
    }

    if(weap_set_id == 0) {
        die("Could not find common event named", weap_set_name);
    }

    auto& weap_reset_ce = Data::commonevents[weap_reset_id -1];
    auto& weap_set_ce = Data::commonevents[weap_set_id -1];

    logInf("Found Common Events ", weap_reset_ce, " and ", weap_set_ce);

    //auto& wr_cmds = weap_reset_ce.event_commands;
    //auto& ws_cmds = weap_set_ce.event_commands;

    CmdBuffer wr_cmds;
    CmdBuffer ws_cmds;

    wr_cmds.push_back(Cmd::Comment("@@GENERATED EVENT! DO NOT EDIT!@@"));
    ws_cmds.push_back(Cmd::Comment("@@GENERATED EVENT! DO NOT EDIT!@@"));

    for(auto act_id = act_info.first_not_ayen_id; act_id <= act_info.num_actors; ++act_id) {
        auto act_idx = act_id - 1;
        auto& actor = Data::actors[act_idx];
        auto& wmap = actors_wmap[act_idx];

        if(wmap.empty()) {
            //Don't generate event code if no mappings
            continue;
        }

        ws_cmds.push_back(Cmd::ConditionalBranch::Actor::IsInParty(act_id, false));

        ws_cmds.push_back(Cmd::ControlVars::Single::Player(eVarMenuLocal1, Cmd::ControlVars::eSet, act_id, Cmd::ControlVars::eWeaponID));
        ws_cmds.push_back(Cmd::ControlVars::Single::Player(eVarMenuLocal2, Cmd::ControlVars::eSet, act_id, Cmd::ControlVars::eShieldID));
        wr_cmds.push_back(Cmd::ControlVars::Single::Player(eVarMenuLocal1, Cmd::ControlVars::eSet, act_id, Cmd::ControlVars::eWeaponID));
        wr_cmds.push_back(Cmd::ControlVars::Single::Player(eVarMenuLocal2, Cmd::ControlVars::eSet, act_id, Cmd::ControlVars::eShieldID));

        genWeaponSwitch(act_id, wmap, wr_cmds, ws_cmds, true);
        if(actor.two_weapon) {
            genWeaponSwitch(act_id, wmap, wr_cmds, ws_cmds, false);
        }

        ws_cmds.push_back(Cmd::END());
        ws_cmds.push_back(Cmd::EndBranch());

    }

    if (isLogLevelEnabled(eLogDebug)) {
        logDbg("Command buffer for ", weap_reset_ce);
        wr_cmds.logCmds(eLogDebug);
        logDbg("");

        logDbg("Command buffer for ", weap_set_ce);
        ws_cmds.logCmds(eLogDebug);
        logDbg("");
    }

    logInf("Assigning commands for ", weap_reset_ce, " cmds");
    weap_reset_ce.event_commands = wr_cmds.take();

    logInf("Assigning commands for ", weap_set_ce, " cmds");
    weap_set_ce.event_commands = ws_cmds.take();

}

void doWeaponCopies() {
    auto act_info = getActorInfo();
    auto amap = makeWeaponAnimationMap(act_info);
    auto actors_wmap = createWeaponCopies(act_info, amap);
    regenCommonEvents(act_info, actors_wmap);

}
