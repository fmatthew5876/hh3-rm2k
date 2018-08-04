#pragma once
#include <rpg_map.h>
#include <rpg_mapinfo.h>

struct MapCache {
    static void setMapDir(std::string d);
    static void setEncoding(std::string e);

    static RPG::Map& loadMap(const RPG::MapInfo& map_info);
    static void saveMap(const RPG::MapInfo& map_info, const RPG::Map& map);

    static void clear();
};
