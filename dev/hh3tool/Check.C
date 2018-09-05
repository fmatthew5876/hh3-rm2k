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

    void onEnemy(const RPG::Enemy& enemy) const {
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
