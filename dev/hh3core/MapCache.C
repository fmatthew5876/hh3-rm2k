#include "MapCache.H"
#include "Exception.H"
#include "Log.H"
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iomanip>
#include <lcf/lmu/reader.h>
#include <mutex>

namespace {
std::string gMapDir;
std::string gEncoding;
std::unordered_map<int,std::unique_ptr<lcf::rpg::Map>> gMapCache;
std::mutex gMutex;
}

void MapCache::setMapDir(std::string d) {
    std::lock_guard<std::mutex> lock(gMutex);
    if (!gMapCache.empty()) {
        die("MapCache: Cannot change map dir after loading maps!");
    }
    gMapDir = std::move(d);
}

void MapCache::setEncoding(std::string e) {
    std::lock_guard<std::mutex> lock(gMutex);
    if (!gMapCache.empty()) {
        die("MapCache: Cannot change map dir after loading maps!");
    }
    gMapDir = std::move(e);
}


const std::string& MapCache::getEncoding() {
	return gEncoding;
}

lcf::rpg::Map& MapCache::loadMap(const lcf::rpg::MapInfo& map_info) {
    std::lock_guard<std::mutex> lock(gMutex);

    if (map_info.type != 1) {
        die("Tried to load map data for map ", map_info, " with type=", map_info.type);
    }

    auto iter = gMapCache.find(map_info.ID);
    if (iter != gMapCache.end()) {
        return *iter->second;
    }

    std::ostringstream ss;
    ss << gMapDir << "/Map" << std::setfill('0') << std::setw(4) << map_info.ID << ".lmu";
    auto map_path = ss.str();

    logDbg("Loading map file `", map_path, "' with encoding=", gEncoding, " ...");

    auto map = lcf::LMU_Reader::Load(map_path, gEncoding);
    if (!map) {
        die("Failed to load map file from `", map_path, "'");
    }

    auto p = gMapCache.insert({map_info.ID, std::move(map)});
    return *p.first->second;
}

void MapCache::saveMap(const lcf::rpg::MapInfo& map_info, lcf::rpg::Map& map) {
    std::lock_guard<std::mutex> lock(gMutex);

    if (map_info.type != 1) {
        die("Tried to save map data for map ", map_info, " with type=", map_info.type);
    }

    std::ostringstream ss;
    ss << gMapDir << "/Map" << std::setfill('0') << std::setw(4) << map_info.ID << ".lmu";
    auto map_path = ss.str();

    logDbg("Saving map file `", map_path, "' ...");

    auto rc = lcf::LMU_Reader::Save(map_path, map, "");
    if (!rc) {
        die("Failed to save map file to `", map_path, "'");
    }

    auto p = gMapCache.insert({map_info.ID, nullptr});
    if (p.second) {
        p.first->second = std::make_unique<lcf::rpg::Map>(map);
    } else {
        *p.first->second = map;
    }
}

void MapCache::clear() {
    std::lock_guard<std::mutex> lock(gMutex);

    gMapCache.clear();
}
