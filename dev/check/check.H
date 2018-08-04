#pragma once
#include <data.h>
#include <rpg_map.h>
#include "Error.H"

class CheckVisitor {
    public:
        void onDatabase(const RPG::Database& db) {}

        void onActors(const std::vector<RPG::Actor>& actor) {}
        void onActor(const RPG::Actor& actor) {}

        void onSkills(const std::vector<RPG::Skill>& skills) {}
        void onSkill(const RPG::Skill& skill) {}

        void onItems(const std::vector<RPG::Item>& items) {}
        void onItem(const RPG::Item& item) {}

        void onCommonEvents(const std::vector<RPG::CommonEvent>& ces) {}
        void onCommonEvent(const RPG::CommonEvent& ce) {}
        void onCommonEventCmds(const RPG::CommonEvent& ce, const std::vector<RPG::EventCommand>& cmds) {}
        //onEventCmds(cmds) called after this..
        void onCommonEventCmd(const RPG::CommonEvent& ce, const RPG::EventCommand& cmd) {}
        //onEventCmd(cmds) called after this..

        void onEnemies(const std::vector<RPG::Enemy>& enemies) {}
        void onEnemy(const RPG::Enemy& enemy) {}
        void onEnemyActions(const RPG::Enemy& enemy, const std::vector<RPG::EnemyAction>& actions) {}
        void onEnemyAction(const RPG::Enemy& enemy, const RPG::EnemyAction& action) {}

        void onTroops(const std::vector<RPG::Troop>& troops) {}
        void onTroop(const RPG::Troop& troop) {}
        void onTroopMembers(const RPG::Troop& troop, const std::vector<RPG::TroopMember>& members) {}
        void onTroopMember(const RPG::Troop& troop, const RPG::TroopMember& member) {}
        void onTroopPages(const RPG::Troop& troop, const std::vector<RPG::TroopPage>& page) {}
        void onTroopPage(const RPG::Troop& troop, const RPG::TroopPage& page) {}
        void onTroopPageCmds(const RPG::Troop& troop, const RPG::TroopPage& page, const std::vector<RPG::EventCommand>& cmds) {}
        //onEventCmds(cmds) called after this..
        void onTroopPageCmd(const RPG::Troop& troop, const RPG::TroopPage& page, const RPG::EventCommand& cmd) {}
        //onEventCmd(cmd) called after this..

        void onTerrains(const std::vector<RPG::Terrain>& terrains) {}
        void onTerrain(const RPG::Terrain& terrain) {}

        void onAttributes(const std::vector<RPG::Attribute>& attrs) {}
        void onAttribute(const RPG::Attribute& attr) {}

        void onStates(const std::vector<RPG::State>& state) {}
        void onState(const RPG::State& state) {}

        void onAnimations(const std::vector<RPG::Animation>& anims) {}
        void onAnimation(const RPG::Animation& anim) {}

        void onChipsets(const std::vector<RPG::Chipset>& chipsets) {}
        void onChipset(const RPG::Chipset& chipset) {}

        void onBattleCommands(const RPG::BattleCommands& batcmds) {}
        void onBattleCommandsCmds(const RPG::BattleCommands& batcmds, const std::vector<RPG::BattleCommand>& cmds) {}
        void onBattleCommand(const RPG::BattleCommands& batcmds, const RPG::BattleCommand& batcmd) {}

        void onClasses(const std::vector<RPG::Class>& classes) {}
        void onClass(const RPG::Class& cls) {}

        void onBattlerAnimations(const std::vector<RPG::BattlerAnimation>& batanims) {}
        void onBattlerAnimation(const RPG::BattlerAnimation& batanim) {}

        void onTerms(const RPG::Terms& terms) {}

        void onSystem(const RPG::System& sys) {}

        void onSwitches(const std::vector<RPG::Switch>& sws) {}
        void onSwitch(const RPG::Switch& sw) {}

        void onVariables(const std::vector<RPG::Variable>& vars) {}
        void onVariable(const RPG::Variable& var) {}

        void onTreeMap(const RPG::TreeMap& tm) {}

        void onMapRoot(const RPG::MapInfo& map_info) {}

        void onMap(const RPG::MapInfo& map_info, const RPG::Map& map) {}
        void onMapEvents(const RPG::MapInfo& map_info, const RPG::Map& map, const std::vector<RPG::Event>& events) {}
        void onMapEvent(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event) {}
        void onMapEventPages(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const std::vector<RPG::EventPage>& pages) {}
        void onMapEventPage(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const RPG::EventPage& page) {}
        void onMapEventPageCmds(const RPG::MapInfo& map_info, const RPG::Map& map,
                const RPG::Event& event, const RPG::EventPage& page,
                const std::vector<RPG::EventCommand>& cmds) {}
        //onEventCmds(cmds) called after this..
        void onMapEventPageCmd(const RPG::MapInfo& map_info, const RPG::Map& map,
                const RPG::Event& event, const RPG::EventPage& page,
                const RPG::EventCommand& cmd) {}
        //onEventCmd(cmds) called after this..

        void onAreaMap(const RPG::MapInfo& map_info) {}

        void onEventCommands(const std::vector<RPG::EventCommand>& cmds) {}
        void onEventCommand(const RPG::EventCommand& cmds) {}
    private:
        ErrorSet _err;
};



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

