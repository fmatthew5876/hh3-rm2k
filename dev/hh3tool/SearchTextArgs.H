#pragma once
#include <string>
#include "CmdLineArgs.H"

struct SearchTextArgs {
    static constexpr auto name = "searchtext";
    static SearchTextArgs load(CmdLineArgs argv);
    void usage();

    std::string text;
    bool is_case_sensitive = false;
};
