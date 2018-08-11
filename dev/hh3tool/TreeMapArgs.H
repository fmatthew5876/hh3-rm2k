#pragma once
#include <string>
#include "CmdLineArgs.H"

struct TreeMapArgs {
    static constexpr auto name = "treemap";
    static TreeMapArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const TreeMapArgs& args);

    std::string map_nameid;
};

