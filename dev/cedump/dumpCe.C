#include "Log.H"
#include "Exception.H"
#include "Code.H"
#include <data.h>

static void dumpCommand(int idx, const RPG::EventCommand& cmd) {
    logInfStart("Cmd (", idx, ") : code=", Code(cmd.code), " indent=", cmd.indent, " str=\"", cmd.string, "\" params=");
    char sep = '[';
    if(cmd.parameters.empty()) {
        logInfRaw(sep);
    } else {
        for (auto& p: cmd.parameters) {
            logInfRaw(sep, p);
            sep = ' ';
        }
    }
    logInfEnd(']');
}

void dumpCommonEvent(int event_id) {
    if(event_id <= 0 || event_id > Data::commonevents.size()) {
        die("Invalid Common Event id (", event_id, ")");
    }

    auto& ce = Data::commonevents[event_id-1];
    auto& trigger_sw = Data::switches[ce.switch_id-1];

    auto& cmds = ce.event_commands;

    logInf("Common Event ", ce, " trigger=", ce.trigger, " swflag=", ce.switch_flag, " switch=", trigger_sw, " num_cmds=", cmds.size()); 
    int idx = 0;
    for(auto& cmd: cmds) {
        dumpCommand(idx, cmd);
        ++idx;
    }
}
