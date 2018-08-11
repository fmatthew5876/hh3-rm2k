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

    void onEventCommand(const RPG::EventCommand& cmd) {
        if (cmd.code == RPG::EventCommand::Code::ChangeItems) {
            if (cmd.parameters[1] == 0 && cmd.parameters[2] == item->ID) {
                std::cout << loc() << " : " << item->name << std::endl;
            }
        }
        if (!tool_args->no_shop) {
            if (cmd.code == RPG::EventCommand::Code::OpenShop) {
                for (int i = 4; i < cmd.parameters.size(); ++i) {
                    if (cmd.parameters[i] == item->ID) {
                        std::cout << loc() << " : " << item->name << std::endl;
                    }
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
