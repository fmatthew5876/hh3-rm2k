#pragma once
#include <string>
#include "CmdLineArgs.H"

struct CheckArgs {
    static constexpr auto name = "check";
    static CheckArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const CheckArgs& args);
};

