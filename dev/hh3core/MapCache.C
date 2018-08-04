#include "MapCache.H"
#include "Exception.H"
#include "Log.H"
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <lmu_reader.h>

namespace {
std::string gGameDir;
std::unordered_map<int,std::unique_ptr<RPG::Map>> gMapCache;
}

void MapCache::setGameDir(std::string d) {
    logInf("MapCache: Set GameDir to `", d, "'");
    gGameDir = std::move(d);
}

RPG::Map& MapCache::load(const RPG::MapInfo& map_info) {
    if (map_info.type != 1) {
        die("Tried to load map data for map ", map_info, " with type=", map_info.type);
    }

    auto iter = gMapCache.find(map_info.ID);
    if (iter != gMapCache.end()) {
        return *iter->second;
    }

    std::ostringstream ss;
    ss << gGameDir << "/Map" << std::setfill('0') << std::setw(4) << map_info.ID << ".lmu";
    auto map_path = ss.str();

    logDbg("Loading map file `", map_path, "' ...");

    auto map = LMU_Reader::Load(map_path, "");
    if (!map) {
        die("Failed to load map file from `", map_path, "'");
    }

    auto p = gMapCache.insert({map_info.ID, std::move(map)});
    return *p.first->second;
}

void MapCache::save(const RPG::MapInfo& map_info, const RPG::Map& map) {
    if (map_info.type != 1) {
        die("Tried to save map data for map ", map_info, " with type=", map_info.type);
    }

    std::ostringstream ss;
    ss << gGameDir << "/Map" << std::setfill('0') << std::setw(4) << map_info.ID << ".lmu";
    auto map_path = ss.str();

    logDbg("Saving map file `", map_path, "' ...");

    auto rc = LMU_Reader::Save(map_path, map, "");
    if (!rc) {
        die("Failed to save map file to `", map_path, "'");
    }

    auto p = gMapCache.insert({map_info.ID, nullptr});
    if (p.second) {
        p.first->second = std::make_unique<RPG::Map>(map);
    } else {
        *p.first->second = map;
    }
}

void MapCache::clear() {
    gMapCache.clear();
}
