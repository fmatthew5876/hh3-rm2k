#pragma once
#include <string>
#include "CmdLineArgs.H"

struct FindVarArgs {
    static constexpr auto name = "findvar";
    static FindVarArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const FindVarArgs& args);

    std::string nameid;
    bool do_troops = true;
    bool do_commonevents = true;
    bool do_maps = true;
};

