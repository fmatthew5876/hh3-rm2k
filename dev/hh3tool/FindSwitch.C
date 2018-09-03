#include "Visit.H"
#include "Apply.H"
#include "FindSwitchArgs.H"
#include "MapCache.H"
#include "MoveCmd.H"
#include <cstring>
#include <iostream>
#include <iomanip>

struct FindSwitchVisitor : public VisitorBase {
    const FindSwitchArgs* tool_args = nullptr;
    const RPG::Switch* sw = nullptr;

    template <typename... Args>
    void found(Args&&... args) const {
        std::cout << loc() << " : SW:" << *sw;
        (std::cout << ... << args);
        std::cout << std::endl;
    }

    void onSkill(const RPG::Skill& skill) {
        if (skill.type == RPG::Skill::Type_switch && skill.switch_id == sw->ID) {
            found();
        }
    }

    void onItem(const RPG::Item& item) const {
        if (item.type == RPG::Item::Type_switch && item.switch_id == sw->ID) {
            found();
        }
    }

    void onCommonEvent(const RPG::CommonEvent& ce) const {
        if (ce.switch_flag && ce.switch_id == sw->ID) {
            found(" condition");
        }
    }

    void onEnemyAction(const RPG::Enemy& enemy, const RPG::EnemyAction& action) const {
        if (action.switch_off && action.switch_off_id == sw->ID) {
            found(" switch off");
        }
        if (action.switch_on && action.switch_on_id == sw->ID) {
            found(" switch on");
        }
    }

    void onTroopPage(const RPG::Troop& troop, const RPG::TroopPage& page) const {
        auto& c = page.condition;
        if (c.flags.switch_a && c.switch_a_id == sw->ID) {
            found(" condition switch A");
        }
        if (c.flags.switch_b && c.switch_b_id == sw->ID) {
            found(" condition switch B");
        }
    }

    void onMapEventPage(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const RPG::EventPage& page) const {
        auto& c = page.condition;
        if (c.flags.switch_a && c.switch_a_id == sw->ID) {
            found(" condition switch A");
        }
        if (c.flags.switch_b && c.switch_b_id == sw->ID) {
            found(" condition switch B");
        }
    }

    void onEventCommand(const RPG::EventCommand& cmd) {
        if (cmd.code == RPG::EventCommand::Code::ControlSwitches) {
            if (cmd.parameters[0] == 0 && cmd.parameters[1] == sw->ID) {
                found();
            }
            if (cmd.parameters[0] == 1 && cmd.parameters[1] <= sw->ID && cmd.parameters[2] >= sw->ID) {
                found();
            }
        }
        if (cmd.code == RPG::EventCommand::Code::ConditionalBranch || cmd.code == RPG::EventCommand::Code::ConditionalBranch_B) {
            if (cmd.parameters[0] == 0 && cmd.parameters[1] == sw->ID) {
                found();
            }
        }
        if (cmd.code == RPG::EventCommand::Code::TeleportTargets) {
            if (cmd.parameters[4] == 1 && cmd.parameters[5] == sw->ID) {
                found();
            }
        }
        if (cmd.code == RPG::EventCommand::Code::EscapeTarget) {
            if (cmd.parameters[3] == 1 && cmd.parameters[4] == sw->ID) {
                found();
            }
        }
        if (cmd.code == RPG::EventCommand::Code::MoveEvent) {
            auto findSw = [&](auto cmd) {
                if (cmd[0] == 32 || cmd[0] == 33) {
                    auto id = getSwitchMoveCmdSwitchId(cmd);
                    if (id == sw->ID) {
                        found(cmd[0] == 32 ? " Move SW ON" : " Move SW OFF");
                    }
                }
            };
            applyMoveCommands(cmd.parameters, findSw);
        }
    }
};


void doFindSwitch(const FindSwitchArgs& tool_args) {
    FindSwitchVisitor v;
    v.tool_args = &tool_args;

    applySwitches(tool_args.nameid,
            [&](const auto& sw) {
                v.sw = &sw;
                visitRPG(v);
            });
}

void FindSwitchArgs::doTool(const FindSwitchArgs& tool_args) {
    doFindSwitch(tool_args);
}
