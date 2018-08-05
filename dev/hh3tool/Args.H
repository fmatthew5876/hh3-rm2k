#pragma once
#include <variant>

#include "SearchTextArgs.H"
#include "Log.H"
#include "Load.H"
#include "CmdLineArgs.H"

using ToolArgs = std::variant<SearchTextArgs>;

struct Args {
    static void usage();
    static Args load(CmdLineArgs argv);

    ToolArgs tool_args;
    LoadArgs load_args;
    LogLevel log_level = eLogInfo;
};

Args loadArgs(int argc, char** argv);
void usage(const Args& args);
