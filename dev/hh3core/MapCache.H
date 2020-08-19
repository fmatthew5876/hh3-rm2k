#pragma once
#include <lcf/rpg/map.h>
#include <lcf/rpg/mapinfo.h>
#include <string>

struct MapCache {
    static void setMapDir(std::string d);
    static void setEncoding(std::string e);
	static const std::string& getEncoding();

    static lcf::rpg::Map& loadMap(const lcf::rpg::MapInfo& map_info);
    static void saveMap(const lcf::rpg::MapInfo& map_info, lcf::rpg::Map& map);

    static void clear();
};
