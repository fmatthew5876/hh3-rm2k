#pragma once
#include <string>
#include "CmdLineArgs.H"

struct FindCallArgs {
    static constexpr auto name = "findcall";
    static FindCallArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const FindCallArgs& args);

    std::string nameid;
};

