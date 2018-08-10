#pragma once
#include <variant>

#include "GrepArgs.H"
#include "Log.H"
#include "Load.H"
#include "CmdLineArgs.H"

using ToolArgs = std::variant<GrepArgs>;

struct Args {
    static void usage();
    static Args load(CmdLineArgs& argv);
    static void dispatchTool(const Args& args);

    ToolArgs tool_args;
    LoadArgs load_args;
    LogLevel log_level = eLogInfo;
};

