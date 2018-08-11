#pragma once
#include <string>
#include "CmdLineArgs.H"
#include <variant>

struct EvDumpArgs {
    static constexpr auto name = "evdump";
    static EvDumpArgs load(CmdLineArgs& argv);
    static void usage();
    static void doTool(const EvDumpArgs& args);

    struct UnknownArgs {};
    struct CArgs {
        std::string nameid;
    };
    struct TArgs {
        std::string nameid;
        int page_num = 0;
    };
    struct MArgs {
        std::string nameid;
        std::string event_nameid;
        int page_num = 0;
    };

    std::variant<UnknownArgs,CArgs,TArgs,MArgs> sub_tool_args = UnknownArgs();
};

