#pragma once
#include <string>
#include "CmdLineArgs.H"

struct FindItemArgs {
    static constexpr auto name = "finditem";
    static FindItemArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const FindItemArgs& args);

    std::string nameid;
    bool no_shop = false;
};

