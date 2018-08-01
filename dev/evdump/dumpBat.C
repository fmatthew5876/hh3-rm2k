#include "Log.H"
#include "Exception.H"
#include "Code.H"
#include "Cmd.H"
#include <data.h>

void dumpBattleEvent(int troop_id, int page_id) {
    if(troop_id <= 0 || troop_id > Data::troops.size()) {
        die("Invalid Troop id (", troop_id, ")");
    }

    auto& troop = Data::troops[troop_id-1];
    if(page_id <= 0 || page_id > troop.pages.size()) {
        die("Invalid Page id (", page_id, ") for  Troop (", troop_id, ")");
    }

    auto& page = troop.pages[page_id - 1];

    auto& cmds = page.event_commands;

    logInf("Troop ", troop, " Page (", page_id, ")  num_cmds=", cmds.size()); 
    int idx = 0;
    for(auto& cmd: cmds) {
        Cmd::logCmd(idx, cmd);
        ++idx;
    }
}
