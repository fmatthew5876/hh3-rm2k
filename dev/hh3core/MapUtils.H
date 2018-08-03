#pragma once
#include <data.h>
#include <algorithm>
#include "Exception.H"

template <typename F>
void visitTreeMap(F&& f) {

    auto& maps = Data::treemap.maps;
    auto& tree_order= Data::treemap.tree_order;

    for (auto& map_id: tree_order) {
        auto iter = std::lower_bound(maps.begin(), maps.end(), map_id,
                [](const auto& map_info, int map_id) { return map_info.ID < map_id; });
        if (iter == maps.end() || iter->ID != map_id) {
            die("Invalid MapID (", map_id, ") in the tree map?? Corruption??");
        }

        f(*iter);
    }
}
