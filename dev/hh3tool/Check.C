#include "Visit.H"
#include "Apply.H"
#include "CheckArgs.H"
#include "MapCache.H"
#include "Constants.H"
#include <cstring>
#include <iostream>
#include <iomanip>

struct CheckVisitor : public VisitorBase {
    const CheckArgs* tool_args = nullptr;

    template <typename... Args>
    void bug(Args&&... args) const {
        std::cout << loc() << " : BUG : ";
        (std::cout << ... << args);
        std::cout << std::endl;
    }

    void onActor(const RPG::Actor& actor) const {
    }

    void onItem(const RPG::Item& item) const {
        auto id = item.ID;
        if (item.ID >= hh3::eItWeaponStart && item.ID <= hh3::eItLastWeapon) {
            if (item.type == RPG::Item::Type_weapon) {
                auto& as = item.attribute_set;
                int num_weapon_attrs
                    = as[hh3::eAtSlashing-1]
                    + as[hh3::eAtPiercing-1]
                    + as[hh3::eAtBlunt-1]
                    + as[hh3::eAtMissile-1];
                if (num_weapon_attrs == 0) {
                    bug("Weapon has no damage type!");
                }
                if (num_weapon_attrs > 1) {
                    bug("Weapon has multiple damage types!");
                }
                const auto& desc = item.description;
                if (desc.size() < 11 || desc[3] != ' ' || desc[9] != ':' || desc[10] != ' ') {
                    bug("Weapon has invalid description: `", desc, "'");
                }
                auto type = desc.substr(0, 4);
                if (
                        (hh3::isKnife(id) && type != "KNF ")
                        || (hh3::isSword(id) && type != "SWD ")
                        || (hh3::isMace(id) && type != "CLB ")
                        || (hh3::isWand(id) && type != "WND ")
                        || (hh3::isStaff(id) && type != "STF ")
                        || (hh3::isAxe(id) && type != "AXE ")
                        || (hh3::isSpear(id) && type != "SPR ")
                        || (hh3::isHalberd(id) && type != "HLB ")
                        || (hh3::isNinjaWeapon(id) && type != "NIN ")
                        || (hh3::isKatana(id) && type != "KAT ")
                        || (hh3::isGun(id) && type != "GUN ")
                   ) {
                    bug("Weapon has wrong type prefix in description: `", desc, "'");
                }
                auto is2h = item.two_handed;
                if (is2h && desc.substr(4,2) != "2H") {
                    bug("Weapon is 2 handed but missing 2H prefix");
                }
                if (!is2h && desc.substr(4,2) != "1H") {
                    bug("Weapon is 1 handed but missing 1H prefix");
                }
                if (as[hh3::eAtSlashing-1] && desc[6] != 'S') {
                    bug("Weapon is Slashing but missing S prefix");
                }
                if (as[hh3::eAtPiercing-1] && desc[6] != 'P') {
                    bug("Weapon is Piercing but missing P prefix");
                }
                if (as[hh3::eAtBlunt-1] && desc[6] != 'B') {
                    bug("Weapon is Blunt but missing B prefix");
                }
                if (as[hh3::eAtMissile-1] && desc[6] != 'M') {
                    bug("Weapon is Missile but missing M prefix");
                }
                auto hit = item.hit;
                if (desc.substr(7,2) != std::to_string(hit)) {
                    bug("Weapon is missing hit prefix");
                }

            }
        }
    }

    void onEnemy(const RPG::Enemy& enemy) const {
    }

    void onTroopMember(const RPG::Troop& troop, const RPG::TroopMember& member) const {
        if (member.y < 72) {
            bug("Enemy is placed too high for gun animations! Y must be >= 72");
        }
    }

    void onTreeMap(const RPG::TreeMap& tm) const {
        auto& st = tm.start;
        if (st.party_map_id != 693 || st.party_x != 9 || st.party_y != 8) {
            bug("Wrong Party Start Position Map=", st.party_map_id, " X=", st.party_x, " Y=", st.party_y);
        }
    }

    void onMapEventPage(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const RPG::EventPage& page) const {
    }

    void onEventCommands(const std::vector<RPG::EventCommand>& cmds) const {

        auto* map_info = getPtr<RPG::MapInfo>(this->loc());
        auto map_id = map_info ? map_info->ID : 0;
        auto* map_evt = getPtr<RPG::Event>(this->loc());
        auto* evt_page = getPtr<RPG::EventPage>(this->loc());

        //Check for bad teleports
        bool screen_visible = true;
        for (auto& c: cmds) {
            auto& p = c.parameters;
            if (c.code == RPG::EventCommand::Code::EraseScreen) {
                screen_visible = false;
            }
            if (c.code == RPG::EventCommand::Code::ShowScreen) {
                screen_visible = true;
            }
            if (c.code == RPG::EventCommand::Code::CallEvent) {
                //Some teleport common event helpers turn off the screen for us.
                if (p[0] == 0) {
                    if (p[1] == hh3::eCeLadderUp
                            || p[1] == hh3::eCeLadderUpStayClimbing
                            || p[1] == hh3::eCeLadderDown
                            || p[1] == hh3::eCeLadderDownStayClimbing) {
                    }
                    screen_visible = false;
                }
            }
            if (c.code == RPG::EventCommand::Code::Teleport) {
                if (screen_visible) {
                    //Teleports to same map don't require hide screen
                    auto target_map = c.parameters[0];
                    if (map_id != target_map) {
                        bug("Teleport without hide screen!");
                    }
                }
            }
        }

        bool is_menu_mode = false;
        bool had_shop = false;
        //Check for bad shops
        for (auto& c: cmds) {
            auto& p = c.parameters;
            if (c.code == RPG::EventCommand::Code::CallEvent) {
                if (p[0] == 0) {
                    if (p[1] == hh3::eCePREMENU_EquipSkills) {
                        is_menu_mode = true;
                    }
                    if (p[1] == hh3::eCePREGAME_EquipSkills) {
                        is_menu_mode = false;
                    }
                }
            }
            if (c.code == RPG::EventCommand::Code::OpenShop) {
                had_shop = true;
                for (int i = 4; i < p.size(); ++i) {
                    auto& item = Data::items[p[i]-1];
                    if (item.type == RPG::Item::Type_weapon) {
                        if (!is_menu_mode) {
                            bug("Shop sells weapons but we didn't call PREMENU CE first!");
                        }
                        break;
                    }
                }
            }
        }
        if (had_shop && is_menu_mode) {
            bug("We setup PREMENU before the shop but didn't call PREGAME after!");
        }

        if (map_evt && map_evt->name == "MAP SETUP") {
            int num_map_changes = 0;
            bool got_tint_before_map = false;
            //Check for incorrect MapPrevMapusage
            for (auto& c: cmds) {
                auto& p = c.parameters;
                if (c.code == RPG::EventCommand::Code::TintScreen) {
                    if (num_map_changes == 0) {
                        got_tint_before_map = true;
                    }
                }
                if (c.code == RPG::EventCommand::Code::CallEvent) {
                    if (p[0] == 0) {
                        if (p[1] == hh3::eCeONMAP_OUT
                                || p[1] == hh3::eCeONMAP_IN
                                || p[1] == hh3::eCeONMAP_DUNGOUT
                                || p[1] == hh3::eCeONMAP_DUNGIN
                                || p[1] == hh3::eCeONMAP_WORLD) {
                            ++num_map_changes;
                        }
                    }
                }
                if (c.code == RPG::EventCommand::Code::ControlSwitches) {
                    if (num_map_changes > 0) {
                        auto isBad = [&](int id) {
                            return id == hh3::eSwMAPColdSnow
                                || (id == hh3::eSwMAPCustomTint && got_tint_before_map)
                                || id == hh3::eSwMAPUnderWater
                                || id == hh3::eSwMAPIndoorWeather;
                        };
                        if (p[0] == 0 && isBad(p[1])) {
                            bug("Changed switch ", Data::switches[p[1]-1], " after map change event!");
                        }
                    }
                }
                if (c.code == RPG::EventCommand::Code::ControlVars) {
                    if (num_map_changes > 0) {
                        auto isBad = [](int id) {
                            return id == hh3::eVaMAPIndoorLightLevel || id == hh3::eVaMAPRainStrength;
                        };
                        if (p[0] == 0 && isBad(p[1])) {
                            bug("Changed variable ", Data::variables[p[1]-1], " after map change event!");
                        }
                    }
                }
                if (c.code == RPG::EventCommand::Code::ConditionalBranch) {
                    if (p[0] == 1) {
                        if (num_map_changes == 0) {
                            auto isBad = [](int id) {
                                return id == hh3::eVaMapPrevMap;
                            };
                            if (isBad(p[1])) {
                                bug("Conditional on ", Data::variables[p[1]-1], " (lhs) before calling Map change event! Probably broken!");
                            }
                            if (p[2] == 1 && isBad(p[3])) {
                                bug("Conditional on ", Data::variables[p[3]-1], " (lhs) before calling Map change event! Probably broken!");
                            }
                        } else {
                            auto isBad = [](int id) {
                                return id == hh3::eVaMapThisMap;
                            };
                            if (isBad(p[1])) {
                                bug("Conditional on ", Data::variables[p[1]-1], " (lhs) after calling Map change event! Probably broken!");
                            }
                            if (p[2] == 1 && isBad(p[3])) {
                                bug("Conditional on ", Data::variables[p[3]-1], " (rhs) after calling Map change event! Probably broken!");
                            }
                        }
                    }
                }
            }
            if (num_map_changes == 0 && evt_page->trigger == RPG::EventPage::Trigger_auto_start) {
                if (map_id != 727) { //Durthall Kitchen has autostarts for a proposed mini-game. Not broken here.
                    bug("Never called any map change event for AutoStart MAP SETUP page!");
                }
            }
            if (num_map_changes > 1) {
                bug("Called too many map change events! (", num_map_changes, ")");
            }
        }

        bool stayed_at_inn = false;
        bool cleaned_up_after_inn = false;
        for (auto& c: cmds) {
            auto& p = c.parameters;
            if (c.code == RPG::EventCommand::Code::ShowInn) {
                if (p[1] == 0) {
                    //Inn was free no branch.
                    stayed_at_inn = true;
                    continue;
                }
                if (p[1] > 0 && p[2] == 0) {
                    bug("Inn costs money but is not conditional");
                }
            }
            if (c.code == RPG::EventCommand::Code::Stay) {
                stayed_at_inn = true;
                continue;
            }
            if (c.code == RPG::EventCommand::Code::CallEvent) {
                if (p[0] == 0) {
                    if (p[1] == hh3::eCeRestingCleanupEvent && stayed_at_inn) {
                        cleaned_up_after_inn = true;
                    }
                }
            }
            if (stayed_at_inn && !cleaned_up_after_inn) {
                bug("Stayed at Inn but didn't call cleanup immediately after!");
            }
            stayed_at_inn = false;
            cleaned_up_after_inn = false;
        }
    }

    void onEventCommand(const RPG::EventCommand& cmd) {
        auto& p = cmd.parameters;

        auto* ce = getPtr<RPG::CommonEvent>(this->loc());
        auto* map_evt = getPtr<RPG::Event>(this->loc());

        //CHECK: Must use MANUAL_SAVE CE to save the game
        if (cmd.code == RPG::EventCommand::Code::OpenSaveMenu) {
            if (ce == nullptr || ce->ID != int(hh3::eCeMANUAL_SAVE)) {
                bug("Call to OpenSaveMenu is prohibited! Must use MANUAL_SAVE common event!");
            }
        }
        if (cmd.code == RPG::EventCommand::Code::TeleportTargets) {
            if (map_evt == nullptr) {
                bug("Teleport was not set in a map event!");
            }
            if (p[0] == 0) {
                if (p[4] == 0) {
                    bug("Teleport with no switch!");
                }
                if (p[4] == 1 && p[5] != hh3::eSwTELEPORT) {
                    bug("Teleport with wrong switch!");
                }
            }
        }
        if (cmd.code == RPG::EventCommand::Code::EscapeTarget) {
            if (map_evt == nullptr) {
                bug("Escape was not set in a map event!");
            } else if (map_evt->name != "MAP SETUP") {
                if (p[0] == 694 && p[1] == 12 && p[2] == 22 && map_evt->ID == 20) {
                    //Special exception
                } else {
                    bug("Escape was not set in MAP SETUP!");
                }
            }
            if (p[3] == 0) {
                bug("Escape with no switch!");
            }
            if (p[3] == 1 && p[4] != hh3::eSwESCAPE) {
                bug("Escape with wrong switch!");
            }
        }
    }
};


void doCheck(const CheckArgs& tool_args) {
    CheckVisitor v;
    v.tool_args = &tool_args;

    visitRPG(v);
}

void CheckArgs::doTool(const CheckArgs& tool_args) {
    doCheck(tool_args);
}
