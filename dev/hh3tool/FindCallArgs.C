#include "FindCallArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

FindCallArgs FindCallArgs::load(CmdLineArgs& argv) {
    FindCallArgs tool_args;

    if (argv.empty()) {
        die("No item nameid specified!");
    }

    tool_args.nameid = std::string(argv[0]);
    argv = argv.subspan(1);

    return tool_args;
}

void FindCallArgs::usage() {
    std::cerr << "findcall [OPTS] <NAMEID> - Search for all events which call the specified common event\n";
    std::cerr << std::endl;
}
