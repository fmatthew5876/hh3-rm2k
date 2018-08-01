#include "Log.H"
#include "Exception.H"
#include "Code.H"
#include "Cmd.H"
#include <data.h>

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
        Cmd::logCmd(idx, cmd);
        ++idx;
    }
}
