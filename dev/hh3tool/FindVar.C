#include "Visit.H"
#include "Apply.H"
#include "FindVarArgs.H"
#include "MapCache.H"
#include "MoveCmd.H"
#include <cstring>
#include <iostream>
#include <iomanip>

struct FindVarVisitor : public VisitorBase {
    const FindVarArgs* tool_args = nullptr;
    const RPG::Variable* var = nullptr;

    template <typename... Args>
    void found(Args&&... args) const {
        std::cout << loc() << " : VAR:" << *var;
        (std::cout << ... << args);
        std::cout << std::endl;
    }

    void onTroopPage(const RPG::Troop& troop, const RPG::TroopPage& page) const {
        auto& c = page.condition;
        if (c.flags.variable && c.variable_id == var->ID) {
            found(" condition variable");
        }
    }

    void onMapEventPage(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const RPG::EventPage& page) const {
        auto& c = page.condition;
        if (c.flags.variable && c.variable_id == var->ID) {
            found(" condition switch A");
        }
    }

    void onEventCommand(const RPG::EventCommand& cmd) {
        auto id = var->ID;
        auto& p = cmd.parameters;
        switch (cmd.code) {
            case RPG::EventCommand::Code::ControlVars:
                if (p[0] == 0 && p[1] == id) {
                    found(" lhs single");
                }
                if (p[0] == 1 && p[1] <= id && p[2] >= id) {
                    found(" lhs batch");
                }
                if (p[0] == 2 && p[1] == id) {
                    found(" lhs deref single");
                }
                if (p[4] == 1 && p[5] == id) {
                    found(" rhs");
                }
                if (p[4] == 2 && p[5] == id) {
                    found(" rhs deref");
                }
                break;
            case RPG::EventCommand::Code::InputNumber:
                if (p[1] == id) {
                    found();
                }
                break;
            case RPG::EventCommand::Code::ControlSwitches:
                if (p[0] == 2 && p[1] == id) {
                    found(" rhs switch num");
                }
                break;
            case RPG::EventCommand::Code::TimerOperation:
                if (p[0] == 0 && p[1] == 1 && p[2] == id) {
                    found();
                }
                break;
            case RPG::EventCommand::Code::ChangeGold:
                if (p[1] == 1 && p[2] == id) {
                    found(" how much gold");
                }
                break;
            case RPG::EventCommand::Code::ChangeItems:
                if (p[1] == 1 && p[2] == id) {
                    found(" which item");
                }
                if (p[3] == 1 && p[4] == id) {
                    found(" how many");
                }
                break;
            case RPG::EventCommand::Code::ChangePartyMembers:
                if (p[1] == 1 && p[2] == id) {
                    found(" which actor");
                }
                break;
            case RPG::EventCommand::Code::ChangeExp:
            case RPG::EventCommand::Code::ChangeLevel:
            case RPG::EventCommand::Code::ChangeHP:
            case RPG::EventCommand::Code::ChangeSP:
            case RPG::EventCommand::Code::ChangeSkills:
                if (p[0] == 2 && p[1] == id) {
                    found(" which actor");
                }
                if (p[3] == 1 && p[4] == id) {
                    found(cmd.code == RPG::EventCommand::Code::ChangeSkills ? " which skill" : " how much");
                }
                break;
            case RPG::EventCommand::Code::ChangeParameters:
                if (p[0] == 2 && p[1] == id) {
                    found(" which actor");
                }
                if (p[4] == 1 && p[5] == id) {
                    found(" how much");
                }
                break;
            case RPG::EventCommand::Code::ChangeEquipment:
                if (p[0] == 2 && p[1] == id) {
                    found(" which actor");
                }
                if (p[2] == 0 && p[3] == 1 && p[4] == id) {
                    found(" which item");
                }
                break;
            case RPG::EventCommand::Code::ChangeCondition:
            case RPG::EventCommand::Code::FullHeal:
                if (p[0] == 2 && p[1] == id) {
                    found(" which actor");
                }
                break;
            case RPG::EventCommand::Code::SimulatedAttack:
                if (p[0] == 2 && p[1] == id) {
                    found(" which actor");
                }
                if (p[6] == 1 && p[7] == id) {
                    found(" store dmg");
                }
                break;
            case RPG::EventCommand::Code::EnemyEncounter:
                if (p[0] == 1 && p[1] == id) {
                    found(" which battle");
                }
                break;
            case RPG::EventCommand::Code::MemorizeLocation:
            case RPG::EventCommand::Code::RecallToLocation:
                if (p[0] == id) {
                    found(" mapid");
                }
                if (p[1] == id) {
                    found(" X");
                }
                if (p[2] == id) {
                    found(" Y");
                }
                break;
            case RPG::EventCommand::Code::SetVehicleLocation:
                if (p[1] == 1) {
                    if (p[2] == id) {
                        found(" mapid");
                    }
                    if (p[3] == id) {
                        found(" X");
                    }
                    if (p[4] == id) {
                        found(" Y");
                    }
                }
                break;
            case RPG::EventCommand::Code::ChangeEventLocation:
                if (p[1] == 1) {
                    if (p[2] == id) {
                        found(" X");
                    }
                    if (p[3] == id) {
                        found(" Y");
                    }
                }
                break;
            case RPG::EventCommand::Code::StoreTerrainID:
            case RPG::EventCommand::Code::StoreEventID:
                if (p[0] == 1) {
                    if (p[1] == id) {
                        found(" X");
                    }
                    if (p[2] == id) {
                        found(" Y");
                    }
                }
                if (p[3] == id) {
                    found(" store id");
                }
                break;
            case RPG::EventCommand::Code::ShowPicture:
            case RPG::EventCommand::Code::MovePicture:
                if (p[1] == 1) {
                    if (p[2] == id) {
                        found(" X");
                    }
                    if (p[3] == id) {
                        found(" Y");
                    }
                }
                break;
            case RPG::EventCommand::Code::KeyInputProc:
                if (p[0] == id) {
                    found();
                }
                break;
            case RPG::EventCommand::Code::ConditionalBranch:
            case RPG::EventCommand::Code::ConditionalBranch_B:
                if (p[0] == 1) {
                    if (p[1] == id) {
                        found(" lhs compare");
                    }
                    if (p[2] == 1 && id == p[3]) {
                        found(" rhs compare");
                    }
                }
                break;
            case RPG::EventCommand::Code::CallEvent:
                if (p[0] == 2) {
                    if (p[1] == id) {
                        found(" Event Id");
                    }
                    if (p[2] == id) {
                        found(" Page Number");
                    }
                }
                break;
            case RPG::EventCommand::Code::ChangeMonsterHP:
            case RPG::EventCommand::Code::ChangeMonsterMP:
                if (p[2] == 1 && p[3] == id) {
                    found(" how much");
                }
                break;
            default:
                break;
        }
    }
};


void doFindVar(const FindVarArgs& tool_args) {
    FindVarVisitor v;
    v.tool_args = &tool_args;
    v.do_troops = tool_args.do_troops;
    v.do_commonevents = tool_args.do_commonevents;
    v.do_maps = tool_args.do_maps;

    applyVariables(tool_args.nameid,
            [&](const auto& var) {
                v.var = &var;
                visitRPG(v);
            });
}

void FindVarArgs::doTool(const FindVarArgs& tool_args) {
    doFindVar(tool_args);
}
