#include "Visit.H"
#include "Apply.H"
#include "FindItemArgs.H"
#include "MapCache.H"
#include <cstring>
#include <iostream>
#include <iomanip>

struct FindItemVisitor : public VisitorBase {
    const FindItemArgs* tool_args = nullptr;
    const RPG::Item* item = nullptr;

    template <typename... Args>
    void found(Args&&... args) const {
        std::cout << loc() << " : ITEM:" << *item;
        (std::cout << ... << args);
        std::cout << std::endl;
    }

    void onActor(const RPG::Actor& actor) const {
        auto& eq = actor.initial_equipment;
        if (eq.weapon_id == item->ID) {
            found(" Actor Weapon");
        }
        if (eq.shield_id == item->ID) {
            found(" Actor Shield");
        }
        if (eq.armor_id == item->ID) {
            found(" Actor Armor");
        }
        if (eq.helmet_id == item->ID) {
            found(" Actor Armor");
        }
        if (eq.accessory_id == item->ID) {
            found(" Actor Accessory");
        }
    }

    void onEnemy(const RPG::Enemy& enemy) const {
        if (enemy.drop_id == item->ID) {
            found();
        }
    }

    void onEventCommand(const RPG::EventCommand& cmd) {
        if (cmd.code == RPG::EventCommand::Code::ControlVars) {
            if (cmd.parameters[4] == 4 && cmd.parameters[5] == item->ID) {
                found(" Variable Set to Num in", (cmd.parameters[6] ? " Inv Only" : " Inv+Equipped"));
            }
        }
        if (cmd.code == RPG::EventCommand::Code::ChangeEquipment) {
            if (cmd.parameters[3] == 0 && cmd.parameters[4] == item->ID) {
                found();
            }
        }
        if (cmd.code == RPG::EventCommand::Code::ChangeItems) {
            if (cmd.parameters[1] == 0 && cmd.parameters[2] == item->ID) {
                found();
            }
        }
        if (cmd.code == RPG::EventCommand::Code::ConditionalBranch) {
            if (cmd.parameters[0] == 4 && cmd.parameters[2] == item->ID) {
                found( "On has item");
            }
            if (cmd.parameters[0] == 5 && cmd.parameters[2] == 5 && cmd.parameters[3] == item->ID) {
                found(" On Actor has Item");
            }
        }
        if (!tool_args->no_shop && cmd.code == RPG::EventCommand::Code::OpenShop) {
            for (int i = 4; i < cmd.parameters.size(); ++i) {
                if (cmd.parameters[i] == item->ID) {
                        found();
                }
            }
        }
    }
};


void doFindItem(const FindItemArgs& tool_args) {
    FindItemVisitor v;
    v.tool_args = &tool_args;

    applyItems(tool_args.nameid,
            [&](const auto& item) {
                v.item = &item;
                visitRPG(v);
            });
}

void FindItemArgs::doTool(const FindItemArgs& tool_args) {
    doFindItem(tool_args);
}
