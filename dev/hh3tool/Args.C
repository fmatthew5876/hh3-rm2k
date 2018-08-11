#include "Args.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

void Args::usage() {
    std::cerr << "hh3tool - Various dev tools for the game" << std::endl;
    std::cerr << "Usage: hh3tool [-d] <GAMEDIR> <TOOL> [TOOLOPTS]\n" << std::endl;

    GrepArgs::usage();
    TreeMapArgs::usage();
}

Args Args::load(CmdLineArgs& argv) {
    Args args;

    if (argv.empty()) {
        die("No arguments given!");
    }

    if (!std::strcmp(argv[0], "-d")) {
        args.log_level = eLogDebug;
        argv = argv.subspan(1);
    }

    if (argv.empty()) {
        die("No gamedir given!");
    }

    args.load_args.game_dir = argv[0];
    argv = argv.subspan(1);

    if (argv.empty()) {
        die("No toolname given!");
    }

    const char* tool_name = argv[0];
    argv = argv.subspan(1);

    if (!strcmp(tool_name, GrepArgs::name)) {
        args.tool_args = GrepArgs::load(argv);
    } else if (!strcmp(tool_name, TreeMapArgs::name)) {
        args.tool_args = TreeMapArgs::load(argv);
    } else {
        die("Unknown toolname: `", tool_name, "'");
    }

    return args;
}

void Args::dispatchTool(const Args& args) {
    std::visit([](const auto& tool_args) {
            tool_args.doTool(tool_args);
            }, args.tool_args);
}
