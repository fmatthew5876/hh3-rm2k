#pragma once
#include <vector>
#include <string>
#include <cctype>
#include <data.h>
#include <rpg_map.h>
#include "Exception.H"

template <typename T, typename F>
void applyNameId(const char* objname, const std::vector<T>& objs, const std::string& nameid, F&& f) {
    bool is_number = true;
    for (auto c: nameid) {
        if (!std::isdigit(c)) {
            is_number = false;
            break;
        }
    }

    if (is_number) {
        auto id = std::atoi(nameid.c_str());
        if (id == 0 || id >= objs.size()) {
            die("Invalid id (", id, ") for ", objname);
        }
        f(objs[id-1]);
        return;
    }

    int num_found = 0;
    for (auto& obj: objs) {
        if (obj.name == nameid) {
            f(obj);
            ++num_found;
        }
    }

    if (num_found == 0) {
        die("Could not find ", objname, " with name=`", nameid, "'");
    }
}

template <typename F>
void applyActors(const std::string& nameid, F&& f) {
    applyNameId("Actor", Data::actors, nameid, std::forward<F>(f));
}

template <typename F>
void applySkills(const std::string& nameid, F&& f) {
    applyNameId("Skill", Data::skills, nameid, std::forward<F>(f));
}

template <typename F>
void applyItems(const std::string& nameid, F&& f) {
    applyNameId("Item", Data::items, nameid, std::forward<F>(f));
}

template <typename F>
void applyCommonEvents(const std::string& nameid, F&& f) {
    applyNameId("CommonEvent", Data::commonevents, nameid, std::forward<F>(f));
}

template <typename F>
void applyEnemies(const std::string& nameid, F&& f) {
    applyNameId("Enemy", Data::enemies, nameid, std::forward<F>(f));
}

template <typename F>
void applyTroops(const std::string& nameid, F&& f) {
    applyNameId("Troop", Data::troops, nameid, std::forward<F>(f));
}

template <typename F>
void applyTerrains(const std::string& nameid, F&& f) {
    applyNameId("Terrain", Data::terrains, nameid, std::forward<F>(f));
}

template <typename F>
void applyAttributes(const std::string& nameid, F&& f) {
    applyNameId("Attribute", Data::attributes, nameid, std::forward<F>(f));
}

template <typename F>
void applyStates(const std::string& nameid, F&& f) {
    applyNameId("State", Data::states, nameid, std::forward<F>(f));
}

template <typename F>
void applyAnimations(const std::string& nameid, F&& f) {
    applyNameId("Animation", Data::animations, nameid, std::forward<F>(f));
}

template <typename F>
void applyChipsets(const std::string& nameid, F&& f) {
    applyNameId("Chipset", Data::chipsets, nameid, std::forward<F>(f));
}

template <typename F>
void applyBattleCommands(const std::string& nameid, F&& f) {
    applyNameId("BattleCommand", Data::battlecommands, nameid, std::forward<F>(f));
}

template <typename F>
void applyClasses(const std::string& nameid, F&& f) {
    applyNameId("Class", Data::classes, nameid, std::forward<F>(f));
}

template <typename F>
void applyBattlerAnimations(const std::string& nameid, F&& f) {
    applyNameId("BattlerAnimation", Data::battleranimations, nameid, std::forward<F>(f));
}

template <typename F>
void applySwitches(const std::string& nameid, F&& f) {
    applyNameId("Switch", Data::switches, nameid, std::forward<F>(f));
}

template <typename F>
void applyVariables(const std::string& nameid, F&& f) {
    applyNameId("Variable", Data::variables, nameid, std::forward<F>(f));
}

template <typename F>
void applyMaps(const std::string& nameid, F&& f) {
    auto& treemap = Data::treemap;
    bool is_number = true;
    for (auto c: nameid) {
        if (!std::isdigit(c)) {
            is_number = false;
            break;
        }
    }

    if (is_number) {
        auto id = std::atoi(nameid.c_str());
        auto iter = std::lower_bound(treemap.maps.begin(), treemap.maps.end(), id,
                [](const auto& mi, const auto& id) { return mi.ID < id; });
        if (iter == treemap.maps.end() || iter->ID != id) {
            die("No map found with id=", id);
        }
        auto& map_info = *iter;
        f(map_info);
        return;
    }

    int num_found = 0;
    for (auto& map_info: treemap.maps) {
        if (map_info.name == nameid) {
            f(map_info);
            ++num_found;
        }
    }

    if (num_found == 0) {
        die("Could not find Map with name=`", nameid, "'");
    }

}

template <typename F>
void applyMapEvents(const RPG::Map& map, const std::string& nameid, F&& f) {
    applyNameId("Event", map.events, nameid, std::forward<F>(f));
}
