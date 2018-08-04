#pragma once
#include <rpg_map.h>
#include <rpg_mapinfo.h>

struct MapCache {
    static void setGameDir(std::string d);

    static RPG::Map& loadMap(const RPG::MapInfo& map_info);
    static void saveMap(const RPG::MapInfo& map_info, const RPG::Map& map);

    static void clear();
};
