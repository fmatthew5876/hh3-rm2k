#pragma once
#include <string>
#include "CmdLineArgs.H"

struct GrepArgs {
    static constexpr auto name = "grep";
    static GrepArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const GrepArgs& args);

    std::string pattern;
    bool is_case_sensitive = false;
};

