#include "FindItemArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

FindItemArgs FindItemArgs::load(CmdLineArgs& argv) {
    FindItemArgs tool_args;

    if (!argv.empty() && !strcmp(argv[0], "-s")) {
        tool_args.no_shop = true;
        argv = argv.subspan(1);
    }

    if (argv.empty()) {
        die("No item nameid specified!");
    }

    tool_args.nameid = std::string(argv[0]);
    argv = argv.subspan(1);

    return tool_args;
}

void FindItemArgs::usage() {
    std::cerr << "finditem [OPTS] <NAMEID> - Search for all events which add/remove the given item\n";
    std::cerr << "  -s Ignore shops\n";
    std::cerr << std::endl;
}
