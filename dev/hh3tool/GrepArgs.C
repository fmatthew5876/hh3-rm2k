#include "GrepArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

GrepArgs GrepArgs::load(CmdLineArgs& argv) {
    GrepArgs tool_args;

    if (argv.empty()) {
        die("No search string specified!");
    }

    tool_args.pattern = argv[0];
    argv = argv.subspan(1);

    while (!argv.empty()) {
        auto opt = argv[0];
        argv = argv.subspan(1);

        if (!std::strcmp(opt, "-i")) {
            tool_args.is_case_sensitive = true;
        } else {
            die("Tool: ", name, " Unknown Option `", opt, "'");
        }
    }

    return tool_args;
}

void GrepArgs::usage() {
    std::cerr << "grep [OPTS] - Search the game for text\n";
    std::cerr << "  -i : Make the search case sensitive" << std::endl;
}
