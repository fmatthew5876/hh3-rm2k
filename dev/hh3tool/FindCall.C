#include "Visit.H"
#include "Apply.H"
#include "FindCallArgs.H"
#include "MapCache.H"
#include <cstring>
#include <iostream>
#include <iomanip>

struct FindCallVisitor : public VisitorBase {
    const FindCallArgs* tool_args = nullptr;
    const RPG::CommonEvent* ce = nullptr;

    template <typename... Args>
    void found(Args&&... args) const {
        std::cout << loc() << " : CE:" << *ce;
        (std::cout << ... << args);
        std::cout << std::endl;
    }

    void onEventCommand(const RPG::EventCommand& cmd) {
        auto id = ce->ID;
        auto& p = cmd.parameters;
        if (cmd.code == RPG::EventCommand::Code::CallEvent) {
            if (p[0] == 0 && p[1] == id) {
                found();
            }
        }
    }
};


void doFindCall(const FindCallArgs& tool_args) {
    FindCallVisitor v;
    v.tool_args = &tool_args;

    applyCommonEvents(tool_args.nameid,
            [&](const auto& ce) {
                v.ce = &ce;
                visitRPG(v);
            });
}

void FindCallArgs::doTool(const FindCallArgs& tool_args) {
    doFindCall(tool_args);
}
