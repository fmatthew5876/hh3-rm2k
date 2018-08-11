#include "EvDumpArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

EvDumpArgs EvDumpArgs::load(CmdLineArgs& argv) {
    EvDumpArgs tool_args;

    if (argv.empty()) {
        die("No subtool specified!");
    }

    auto st_name = std::string(argv[0]);
    argv = argv.subspan(1);

    if (st_name == "C" || st_name == "c") {
        CArgs st_args;
        if (argv.empty()) {
            die("No CommonEvent nameid specified!");
        }

        st_args.nameid = argv[0];
        argv = argv.subspan(1);

        tool_args.sub_tool_args = st_args;

    } else if (st_name == "T" || st_name == "t") {
        TArgs st_args;
        if (argv.empty()) {
            die("No Troop nameid specified!");
        }

        st_args.nameid = argv[0];
        argv = argv.subspan(1);

        if (argv.empty()) {
            die("No Troop pagenum specified!");
        }

        st_args.page_num = std::atoi(argv[0]);
        argv = argv.subspan(1);

        tool_args.sub_tool_args = st_args;

    } else if (st_name == "M" || st_name == "m") {
        MArgs st_args;
        if (argv.empty()) {
            die("No Map nameid specified!");
        }

        st_args.nameid = argv[0];
        argv = argv.subspan(1);

        if (argv.empty()) {
            die("No Event nameid specified!");
        }

        st_args.event_nameid = argv[0];
        argv = argv.subspan(1);

        if (argv.empty()) {
            die("No Event pagenum specified!");
        }

        st_args.page_num = std::atoi(argv[0]);
        argv = argv.subspan(1);

        tool_args.sub_tool_args = st_args;
    } else {
        die("Unknown subtool name `", st_name, "'");
    }

    return tool_args;
}

void EvDumpArgs::usage() {
    std::cerr << "evdump C <NAMEID> - Dump Common Event <NAMEID> code\n";
    std::cerr << "evdump T <NAMEID> <PAGENUM> - Dump Troop <NAMEID> Page <PAGENUM> code\n";
    std::cerr << "evdump M <NAMEID> <EVNAMEID> <PAGENUM> - Dump Map <NAMEID> Event <EVNAMEID> Page <PAGENUM> code\n";
    std::cerr << std::endl;
}
