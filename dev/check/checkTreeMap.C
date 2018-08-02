#include "check.H"
#include "Exception.H"
#include "Log.H"

#include <lmu_reader.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>

void checkMapRoot(ErrorSet& err, const RPG::MapInfo& map_info) {
    logInf("Checking MapRoot: ", map_info, " type=", map_info.type, " ... ");
}

void checkAreaMap(ErrorSet& err, const RPG::MapInfo& map_info) {
    logInf("Checking Area Map: ", map_info, " type=", map_info.type, " ... ");
}

void checkTreeMap(ErrorSet& err, const std::string& gamedir) {
    auto& maps = Data::treemap.maps;
    auto& tree_order= Data::treemap.tree_order;

    logInf("Checking TreeMap ...");

    std::cout << "MAPS: " << Data::treemap.maps.size() << " TREE: " << Data::treemap.tree_order.size() << std::endl;

    for (auto& map_id: tree_order) {
        auto iter = std::lower_bound(maps.begin(), maps.end(), map_id,
                [](const auto& map_info, int map_id) { return map_info.ID < map_id; });
        if (iter == maps.end() || iter->ID != map_id) {
            die("Invalid MapID (", map_id, ")");
        }
        auto& map_info = *iter;

        switch(map_info.type) {
            case 0:
                checkMapRoot(err, map_info);
                break;
            case 1:
                {
                    std::ostringstream ss;
                    ss << gamedir << "/Map" << std::setfill('0') << std::setw(4) << map_info.ID << ".lmu";
                    auto map_path = ss.str();

                    auto map = LMU_Reader::Load(map_path, "");
                    if (!map) {
                        die("Failed to load map file `", map_path, "'");
                    }
                    checkMap(err, map_info, *map);
                }
                break;
            case 2:
                checkAreaMap(err, map_info);
                break;
            case 3:
                die("Unknown map type=", map_info.type);
                break;
        }
    }
}
