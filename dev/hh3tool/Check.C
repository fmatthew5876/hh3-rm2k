#include "Visit.H"
#include "Apply.H"
#include "CheckArgs.H"
#include "MapCache.H"
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

    void onMapEventPage(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const RPG::EventPage& page) const {
    }

    void onEventCommand(const RPG::EventCommand& cmd) {
        auto* ce = getPtr<RPG::CommonEvent>(this->loc());

        //CHECK: Must use MANUAL_SAVE CE to save the game
        if (cmd.code == RPG::EventCommand::Code::OpenSaveMenu) {
            if (ce == nullptr || ce->name != "MANUAL_SAVE") {
                bug("Call to OpenSaveMenu is prohibited! Must use MANUAL_SAVE common event!");
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
