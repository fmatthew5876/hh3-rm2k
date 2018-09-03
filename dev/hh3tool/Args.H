#pragma once
#include <variant>

#include "GrepArgs.H"
#include "TreeMapArgs.H"
#include "EvDumpArgs.H"
#include "FindItemArgs.H"
#include "FindSwitchArgs.H"
#include "FindVarArgs.H"
#include "Log.H"
#include "Load.H"
#include "CmdLineArgs.H"

using ToolArgs = std::variant<GrepArgs,TreeMapArgs,EvDumpArgs,FindItemArgs,FindSwitchArgs,FindVarArgs>;

struct Args {
    static void usage();
    static Args load(CmdLineArgs& argv);
    static void dispatchTool(const Args& args);

    ToolArgs tool_args;
    LoadArgs load_args;
    LogLevel log_level = eLogInfo;
};

