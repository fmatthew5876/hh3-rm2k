#include <iostream>
#include <lmt_reader.h>
#include <reader_util.h>
#include <algorithm>

#include "Log.H"
#include "Exception.H"

void dumpCommonEvent(int event_id);
void dumpBattleEvent(int troop_id, int page);
void dumpMapEvent(const std::string& ldb_path, int map_id, int event_id, int page);

void usage() {
    std::cerr << "cedump - Dump map tree info" << std::endl;
    std::cerr << "Usage: maptree [-d] <LMTFILE> - Dump Entire Tree" << std::endl;
    std::cerr << "Usage: maptree [-d] <LMTFILE> <M> - Dump Map Tree from <M> up to the root." << std::endl;
}

void printMap(const RPG::MapInfo& map) {
    for (int i = 0; i < map.indentation; ++i) {
        std::cout << ' ';
    }
    std::cout << map << " parent=" << map.parent_map << std::endl;
}

void printAllTree() {
    auto& maps = Data::treemap.maps;
    auto& tree_order= Data::treemap.tree_order;

    std::cout << "MAPS: " << Data::treemap.maps.size() << " TREE: " << Data::treemap.tree_order.size() << std::endl;

    for (auto& map_id: tree_order) {
        auto iter = std::lower_bound(maps.begin(), maps.end(), map_id,
                [](const auto& map_info, int map_id) { return map_info.ID < map_id; });
        if (iter == maps.end() || iter->ID != map_id) {
            die("Invalid MapID (", map_id, ")");
        }
        auto& map = *iter;

        printMap(map);
    }
}

void printMapTree(const RPG::MapInfo& query_map) {
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

bool is_number(const std::string& s) {
    if (s.empty()) {
        return false;
    }

    for (auto& c: s) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void doMain(const std::string& lmt_path, const std::string& map_name) {
    logInf("Loading LMT tree `", lmt_path, "'...");

    auto rc = LMT_Reader::Load(lmt_path, "");
    if(!rc) {
        throw Exception("Failed to load LMT tree from file `" + lmt_path  + "'");
    }

    if (map_name.empty()) {
        printAllTree();
        return;
    }

    auto& maps = Data::treemap.maps;

    if (is_number(map_name)) {
        auto map_id = std::atoi(map_name.c_str());
        auto iter = std::lower_bound(maps.begin(), maps.end(), map_id,
                [](const auto& map_info, int map_id) { return map_info.ID < map_id; });
        if (iter == maps.end() || iter->ID != map_id) {
            die("Invalid MapID (", map_id, ")");
        }

        printMapTree(*iter);

        return;
    }

    int num_printed = 0;
    for (auto& map: maps) {
        if (map.name == map_name) {
            printMapTree(map);
            ++num_printed;
        }
    }

    if (num_printed  == 0) {
        die("Invalid Map Name (", map_name, ")");
    }

}

int main(int argc, char** argv) {
    if (argc <= 1) {
        usage();
        return 1;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    auto aiter = args.begin();

    if (aiter != args.end() && *aiter == "-d") {
        setLogLevel(eLogDebug);
        ++aiter;
    }

    std::string lmt_path = "";
    std::string map = "";

    try {
        if (aiter == args.end()) {
            die("Invalid arguments");
        }

        lmt_path = *aiter;
        ++aiter;

        if (aiter != args.end()) {
            map = aiter->c_str();
        }

    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        usage();
        return 1;
    }

    try {
        doMain(lmt_path, map);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
