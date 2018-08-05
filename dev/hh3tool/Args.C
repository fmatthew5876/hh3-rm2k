#include "Args.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

void Args::usage() {
    std::cerr << "hh3tool - Various dev tools for the game" << std::endl;
    std::cerr << "Usage: hh3tool [-d] <GAMEDIR> <TOOL>" << std::endl;
}

Args Args::load(CmdLineArgs argv) {
    Args args;

    if (argv.empty()) {
        throw Exception("No arguments given!");
    }

    if (!std::strcmp(argv[0], "-d")) {
        args.log_level = eLogDebug;
        argv = argv.subspan(1);
    }

    if (argv.empty()) {
        throw Exception("No gamedir given!");
    }

    args.load_args.game_dir = argv[0];
    argv = argv.subspan(1);

    if (argv.empty()) {
        throw Exception("No toolname given!");
    }

    const char* tool_name = argv[0];
    argv = argv.subspan(1);

    if (!strcmp(tool_name, SearchTextArgs::name)) {
        args.tool_args = SearchTextArgs::load(argv);
    }

    return args;
}
