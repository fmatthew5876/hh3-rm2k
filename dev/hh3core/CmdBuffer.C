#include "CmdBuffer.H"
#include "Log.H"
#include "Cmd.H"
#include "Exception.H"

void CmdBuffer::push_back(RPG::EventCommand cmd) {

    cmd.indent = _indent;

    switch(cmd.code) {
        case RPG::EventCommand::Code::ConditionalBranch:
        case RPG::EventCommand::Code::ConditionalBranch_B:
        case RPG::EventCommand::Code::ElseBranch:
        case RPG::EventCommand::Code::ElseBranch_B:
        case RPG::EventCommand::Code::Loop:
        case RPG::EventCommand::Code::ShowChoiceOption:
        case RPG::EventCommand::Code::VictoryHandler:
        case RPG::EventCommand::Code::EscapeHandler:
        case RPG::EventCommand::Code::DefeatHandler:
        case RPG::EventCommand::Code::Transaction:
        case RPG::EventCommand::Code::NoTransaction:
        case RPG::EventCommand::Code::Stay:
        case RPG::EventCommand::Code::NoStay:
            ++_indent;
            break;
        case RPG::EventCommand::Code::END:
            --_indent;
            break;
            default:
                break;
    }

    _cmds.push_back(std::move(cmd));

    if (_indent < 0) {
        logErr("Indent went negative! Scopes are probably screwed");
        logCmds(eLogErr);
        _cmds.pop_back();
        die("Invalid command buffer");
    }

}

std::vector<RPG::EventCommand> CmdBuffer::take() {
    if(_cmds.empty()) {
        _indent = 0;
        return {};
    }

    if(_indent != 0) {
        logErr("Unbalanced scopes in command list! final indent = ", _indent);
        logCmds(eLogErr);
        die("Invalid command buffer");
    }

    auto c = std::move(_cmds);
    _indent = 0;

    return c;
}

void CmdBuffer::logCmds(LogLevel ll) {
    int i = 0;
    for(auto& cmd: _cmds) {
        Cmd::logCmd(i, cmd, ll);
        ++i;
    }
}

