#include "Visit.H"
#include "Apply.H"
#include "TreeMapArgs.H"
#include <cstring>
#include <iostream>

static void printMap(const RPG::MapInfo& map) {
    for (int i = 0; i < map.indentation; ++i) {
        std::cout << ' ';
    }
    std::cout << map << " parent=" << map.parent_map << std::endl;
}

static void printAllTree() {
    visitTreeMap(printMap);
}

static void printMapTree(const RPG::MapInfo& query_map) {
    auto& maps = Data::treemap.maps;
    std::vector<const RPG::MapInfo*> stack = { &query_map };

    while (stack.back()->ID != 0) {
        auto& prev = *stack.back();
        auto map_id = prev.parent_map;
        auto iter = std::lower_bound(maps.begin(), maps.end(), map_id,
                [](const auto& map_info, int map_id) { return map_info.ID < map_id; });
        if (iter == maps.end() || iter->ID != map_id) {
            die("Invalid MapID (", map_id, ")");
        }
        auto& map = *iter;

        stack.push_back(&map);
    }

    for (auto iter = stack.rbegin(); iter != stack.rend(); ++iter) {
        printMap(**iter);
    }
}

void doTreeMap(const TreeMapArgs& tool_args) {
    if (tool_args.map_nameid.empty()) {
        visitTreeMap(printMap);
    } else {
        applyMaps(tool_args.map_nameid, printMapTree);
    }
}

void TreeMapArgs::doTool(const TreeMapArgs& tool_args) {
    doTreeMap(tool_args);
}
