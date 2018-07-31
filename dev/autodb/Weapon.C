#include "Weapon.H"
#include "Log.H"
#include "Exception.H"
#include "Item.H"
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

struct WeaponMapping {
    int orig_id = 0;
    int mapped_id = 0;
    int alt_mapped_id = 0;
};

using ActorsWeaponMap = std::vector<std::vector<WeaponMapping>>;

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

            Data::items[copy_idx] = item_copy;
            ++copy_idx;

            logInf("Created item copy for ", actor, " : ", orig_item, " -> ", item_copy);

            WeaponMapping wm;
            wm.orig_id = orig_item.ID;
            wm.mapped_id = item_copy.ID;

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

                wm.alt_mapped_id = item_copy.ID;
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

                wm.alt_mapped_id = item_copy.ID;
            }

            wmap.push_back(wm);
        }

        ++actor_id;
    }

    return actors_wmap;
}

static void regenCommonEvents(const ActorsWeaponMap& actors_wmap) {
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

    auto& wr_cmds = weap_reset_ce.event_commands;
    auto& ws_cmds = weap_set_ce.event_commands;

    wr_cmds.clear();
    ws_cmds.clear();

    RPG::EventCommand comment;
    comment.code = RPG::EventCommand::Code::Comment;
    comment.string = "@@GENERATED EVENT! DO NOT EDIT!@@";
}

void doWeaponCopies() {
    auto act_info = getActorInfo();
    auto amap = makeWeaponAnimationMap(act_info);
    auto actors_wmap = createWeaponCopies(act_info, amap);
    regenCommonEvents(actors_wmap);

}
