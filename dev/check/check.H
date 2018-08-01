#pragma once
#include <data.h>
#include <rpg_map.h>
#include "Error.H"

void checkDatabase(ErrorSet& err);
void checkTreeMap(ErrorSet& err, const std::string& gamedir);
void checkMap(ErrorSet& err, const RPG::MapInfo& map_info, const RPG::Map& map);
void checkMapEvent(ErrorSet& err, const RPG::Map& map, const RPG::MapInfo& map_info, const RPG::Event& event);
void checkEventCommands(ErrorSet& err, const RPG::Map& map, const RPG::MapInfo& map_info, const RPG::Event& event, const RPG::EventPage& page, const std::vector<RPG::EventCommand>& cmds);

void checkCommonEvent(ErrorSet& err, const RPG::CommonEvent& ce);
void checkEventCommands(ErrorSet& err, const RPG::CommonEvent& ce, const std::vector<RPG::EventCommand>& cmds);

void checkTroop(ErrorSet& err, const RPG::Troop& troop);
void checkTroopPage(ErrorSet& err, const RPG::Troop& troop, const RPG::TroopPage& troop_page);
void checkEventCommands(ErrorSet& err, const RPG::Troop& troop, const RPG::TroopPage& troop_page, const std::vector<RPG::EventCommand>& cmds);

