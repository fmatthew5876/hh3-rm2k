#include "CmdBuffer.H"
#include "Log.H"
#include "Cmd.H"
#include "Exception.H"

void CmdBuffer::push_back(lcf::rpg::EventCommand cmd) {

    cmd.indent = _indent;

    switch((lcf::rpg::EventCommand::Code)cmd.code) {
        case lcf::rpg::EventCommand::Code::ConditionalBranch:
        case lcf::rpg::EventCommand::Code::ConditionalBranch_B:
        case lcf::rpg::EventCommand::Code::ElseBranch:
        case lcf::rpg::EventCommand::Code::ElseBranch_B:
        case lcf::rpg::EventCommand::Code::Loop:
        case lcf::rpg::EventCommand::Code::ShowChoiceOption:
        case lcf::rpg::EventCommand::Code::VictoryHandler:
        case lcf::rpg::EventCommand::Code::EscapeHandler:
        case lcf::rpg::EventCommand::Code::DefeatHandler:
        case lcf::rpg::EventCommand::Code::Transaction:
        case lcf::rpg::EventCommand::Code::NoTransaction:
        case lcf::rpg::EventCommand::Code::Stay:
        case lcf::rpg::EventCommand::Code::NoStay:
            ++_indent;
            break;
        case lcf::rpg::EventCommand::Code::END:
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

std::vector<lcf::rpg::EventCommand> CmdBuffer::take() {
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

