#include "TreeMapArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

TreeMapArgs TreeMapArgs::load(CmdLineArgs& argv) {
    TreeMapArgs tool_args;

    if (!argv.empty()) {
        tool_args.map_nameid = argv[0];
        argv = argv.subspan(1);
    }

    return tool_args;
}

void TreeMapArgs::usage() {
    std::cerr << "treemap [MAP] - Print the treemap for the given MAP name or id. Or the entire tree if no map given.\n";
    std::cerr << std::endl;
}
