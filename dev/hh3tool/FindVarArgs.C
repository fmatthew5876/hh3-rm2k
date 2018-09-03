#include "FindVarArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

FindVarArgs FindVarArgs::load(CmdLineArgs& argv) {
    FindVarArgs tool_args;

    if (!argv.empty() && !strcmp(argv[0], "--no-troops")) {
        tool_args.do_troops = false;
        argv = argv.subspan(1);
    }

    if (!argv.empty() && !strcmp(argv[0], "--no-commonevents")) {
        tool_args.do_commonevents = false;
        argv = argv.subspan(1);
    }

    if (!argv.empty() && !strcmp(argv[0], "--no-maps")) {
        tool_args.do_maps = false;
        argv = argv.subspan(1);
    }

    if (argv.empty()) {
        die("No switch nameid specified!");
    }

    tool_args.nameid = std::string(argv[0]);
    argv = argv.subspan(1);

    return tool_args;
}

void FindVarArgs::usage() {
    std::cerr << "findvar [OPTS] <NAMEID> - Search for all events which touch given variable\n";
    std::cerr << "  --no-troops       Don't search in troops\n";
    std::cerr << "  --no-commonevents Don't search in common events\n";
    std::cerr << "  --no-maps         Don't search in maps\n";
    std::cerr << std::endl;
}
