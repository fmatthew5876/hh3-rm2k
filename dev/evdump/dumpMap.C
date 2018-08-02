#include "Log.H"
#include "Exception.H"
#include "Code.H"
#include "Cmd.H"
#include <lmu_reader.h>
#include <lmt_reader.h>
#include <data.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

void dumpMapEvent(const std::string& ldb_path, int map_id, int event_id, int page_id) {
    if(map_id <= 0 || map_id > 9999) {
        die("Invalid Map id (", event_id, ")");
    }

    auto dname = ldb_path.substr(0, ldb_path.rfind('/'));

    auto lmt_path = dname + "/RPG_RT.lmt";

    logInf("Loading LMT tree `", lmt_path, "'...");

    auto rc = LMT_Reader::Load(lmt_path, "");
    if(!rc) {
        throw Exception("Failed to load LMT tree from file `" + lmt_path  + "'");
    }

    auto& maps = Data::treemap.maps;

    auto iter = std::lower_bound(maps.begin(), maps.end(), map_id,
            [](const auto& map_info, int map_id) { return map_info.ID < map_id; });
    if (iter == maps.end() || iter->ID != map_id) {
        die("Invalid MapID (", map_id, ")");
    }

    auto& map_info = *iter;

    if (map_info.type != 1) {
        die("MapInfo ", map_info, " has type=", map_info.type, " which doesn't have events!");
    }

    std::ostringstream ss;
    ss << dname << "/Map" << std::setfill('0') << std::setw(4) << map_id << ".lmu";
    auto map_path = ss.str();

    logInf("Loading Map file `", map_path, "'...");

    auto map = LMU_Reader::Load(map_path, "");
    if (!map) {
        die("Failed to load map file `", map_path, "'");
    }

    if (event_id <= 0 || event_id > map->events.size()) {
        die("Map ", map_info, " has no event with id (", event_id, ")");
    }

    auto& event = map->events[event_id-1];

    if (page_id <= 0 || page_id > event.pages.size()) {
        die("Map ", map_info, " event ", event, " has no page with id (", page_id, ")");
    }

    auto& page = event.pages[page_id - 1];

    auto& cmds = page.event_commands;

    logInf("Map ", map_info, " Event ", event, " coords=(", event.x, ", ", event.y, ") page=", page_id, " num_cmds=", cmds.size());
    int idx = 0;
    for(auto& cmd: cmds) {
        Cmd::logCmd(idx, cmd);
        ++idx;
    }
}
