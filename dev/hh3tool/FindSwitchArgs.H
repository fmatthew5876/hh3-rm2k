#pragma once
#include <string>
#include "CmdLineArgs.H"

struct FindSwitchArgs {
    static constexpr auto name = "findsw";
    static FindSwitchArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const FindSwitchArgs& args);

    std::string nameid;
};

