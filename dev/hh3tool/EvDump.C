#include "Visit.H"
#include "Apply.H"
#include "EvDumpArgs.H"
#include "MapCache.H"
#include <cstring>
#include <iostream>
#include <iomanip>

static void dumpEventCommands(const std::vector<RPG::EventCommand>& cmds) {
    int idx = 0;
    for (auto& cmd: cmds) {
        auto f = std::cout.flags();
        std::cout << std::setfill('0') << std::setw(4) << idx;
        std::cout.flags(f);
        std::cout << ": ";

        for(int i = 0; i < cmd.indent; ++i) {
            std::cout << ' ';
        }
        std::cout << "code=" << Code(cmd.code) << " indent=" << cmd.indent << " str=\"" << cmd.string << "\" params=";

        char sep = '[';
        if(cmd.parameters.empty()) {
            std::cout << sep;
        } else {
            for (auto& p: cmd.parameters) {
                std::cout << sep << p;
                sep = ' ';
            }
        }
        std::cout << "]\n";

        ++idx;
    }
    std::cout << std::endl;
}

static void dumpEvents(const RPG::CommonEvent& ce) {
    auto& trigger_sw = Data::switches[ce.switch_id-1];
    auto& cmds = ce.event_commands;

    std::cout << "Common Event " <<  ce << " trigger=" << ce.trigger << " swflag=" << ce.switch_flag << " switch=" << trigger_sw << " num_cmds=" << cmds.size() << '\n';
    dumpEventCommands(cmds);
}

static void dumpEvents(const RPG::Troop& troop, int page_num) {
    auto& page = troop.pages[page_num - 1];
    auto& cmds = page.event_commands;

    std::cout << "Troop " << troop << " page=" << page_num << "  num_cmds=" << cmds.size() << '\n';

    dumpEventCommands(cmds);
}

static void dumpEvents(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, int page_num) {

    auto& page = event.pages[page_num - 1];
    auto& cmds = page.event_commands;

    std::cout << "Map " << map_info << " Event " << event << " coords=(" << event.x << ", " << event.y << ") page=" << page_num << " num_cmds=" << cmds.size() << '\n';

    dumpEventCommands(cmds);
}

static void dumpEvents(const RPG::MapInfo& map_info, const std::string& event_nameid, int page_num) {
    auto& map = MapCache::loadMap(map_info);

    applyMapEvents(map, event_nameid,
            [&](const auto& ev) { dumpEvents(map_info, map, ev, page_num); });
}

static void doEvDump(const EvDumpArgs& tool_args, const EvDumpArgs::UnknownArgs& sub_tool_args) {
}

static void doEvDump(const EvDumpArgs& tool_args, const EvDumpArgs::CArgs& sub_tool_args) {
    applyCommonEvents(sub_tool_args.nameid, [](const auto& ce) { dumpEvents(ce); });
}

static void doEvDump(const EvDumpArgs& tool_args, const EvDumpArgs::TArgs& sub_tool_args) {
    applyTroops(sub_tool_args.nameid, [&](const auto& t) { dumpEvents(t, sub_tool_args.page_num); });
}

static void doEvDump(const EvDumpArgs& tool_args, const EvDumpArgs::MArgs& sub_tool_args) {
    applyMaps(sub_tool_args.nameid, [&](const auto& t) { dumpEvents(t, sub_tool_args.event_nameid, sub_tool_args.page_num); });
}

void doEvDump(const EvDumpArgs& tool_args) {
    auto& st_var = tool_args.sub_tool_args;
    std::visit([&](const auto& stargs) { doEvDump(tool_args, stargs); }, st_var);
}

void EvDumpArgs::doTool(const EvDumpArgs& tool_args) {
    doEvDump(tool_args);
}
