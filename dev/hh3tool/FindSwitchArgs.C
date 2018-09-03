#include "FindSwitchArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

FindSwitchArgs FindSwitchArgs::load(CmdLineArgs& argv) {
    FindSwitchArgs tool_args;

    if (argv.empty()) {
        die("No switch nameid specified!");
    }

    tool_args.nameid = std::string(argv[0]);
    argv = argv.subspan(1);

    return tool_args;
}

void FindSwitchArgs::usage() {
    std::cerr << "findsw [OPTS] <NAMEID> - Search for all events which add/remove the given sw\n";
    std::cerr << std::endl;
}
