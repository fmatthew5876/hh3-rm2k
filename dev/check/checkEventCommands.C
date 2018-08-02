#include "check.H"
#include "Error.H"

namespace {

void checkBGMLegacyWavTempo(ErrorSet& err, const ErrorContext& ctx, const RPG::EventCommand& cmd) {
    int tempo = 100;
    if (cmd.code == RPG::EventCommand::Code::PlayBGM) {
        tempo = cmd.parameters[2];
    }
    if (cmd.code == RPG::EventCommand::Code::ChangeSystemBGM) {
        tempo = cmd.parameters[3];
    }
    if (tempo != 100) {
        //Valid tempo changes for SFX
        if (cmd.string.substr(0, 2) != "SE") {
            err.push_back(Error(ctx, "Cmd: ", cmd, " plays ", cmd.string, " at a tempo of 50!"));
        }
    }
}

void checkLoneTeleport(ErrorSet& err, const ErrorContext& ctx, const std::vector<RPG::EventCommand>& cmds) {
    if (cmds.size() == 1) {
        if (cmds.front().code == RPG::EventCommand::Code::Teleport) {
            err.push_back(Error(ctx, "Event cmds only single Teleport, no Hide screen!"));
        }
    }
}

void checkEventCommands(ErrorSet& err, const ErrorContext& ctx, const std::vector<RPG::EventCommand>& cmds) {
    checkLoneTeleport(err, ctx, cmds);

    for (auto& cmd: cmds) {
        checkBGMLegacyWavTempo(err, ctx, cmd);
    }

}


} //namespace


void checkEventCommands(ErrorSet& err, const RPG::Map& map, const RPG::MapInfo& map_info, const RPG::Event& event, const RPG::EventPage& page, const std::vector<RPG::EventCommand>& cmds) {
    auto ctx = ErrorContext(MapContext{map_info, event, page});
    checkEventCommands(err, ctx, cmds);
}

void checkEventCommands(ErrorSet& err, const RPG::CommonEvent& ce, const std::vector<RPG::EventCommand>& cmds) {
    auto ctx = ErrorContext(CommonEventContext{ce});
    checkEventCommands(err, ctx, cmds);
}

void checkEventCommands(ErrorSet& err, const RPG::Troop& troop, const RPG::TroopPage& troop_page, const std::vector<RPG::EventCommand>& cmds) {
    auto ctx = ErrorContext(TroopContext{troop, troop_page});
    checkEventCommands(err, ctx, cmds);
}
