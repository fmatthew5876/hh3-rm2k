#pragma once
#include <data.h>
#include <rpg_map.h>
#include <algorithm>
#include "Exception.H"
#include "MapCache.H"

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
//Inherit from this and implement the methods for things you want to visit.
//Note this class is not virtual for performance reasons.
//You can override for const or non-const as you like.
class VisitorBase {
    public:
        void onDatabase(const RPG::Database& db) const {}

        void onActors(const std::vector<RPG::Actor>& actor) const {}
        void onActor(const RPG::Actor& actor) const {}

        void onSkills(const std::vector<RPG::Skill>& skills) const {}
        void onSkill(const RPG::Skill& skill) const {}

        void onItems(const std::vector<RPG::Item>& items) const {}
        void onItem(const RPG::Item& item) const {}

        void onCommonEvents(const std::vector<RPG::CommonEvent>& ces) const {}
        void onCommonEvent(const RPG::CommonEvent& ce) const {}
        void onCommonEventCmds(const RPG::CommonEvent& ce, const std::vector<RPG::EventCommand>& cmds) const {}
        //onEventCmds(cmds) called after this..
        void onCommonEventCmd(const RPG::CommonEvent& ce, const RPG::EventCommand& cmd) const {}
        //onEventCmd(cmds) called after this..

        void onEnemies(const std::vector<RPG::Enemy>& enemies) const {}
        void onEnemy(const RPG::Enemy& enemy) const {}
        void onEnemyActions(const RPG::Enemy& enemy, const std::vector<RPG::EnemyAction>& actions) const {}
        void onEnemyAction(const RPG::Enemy& enemy, const RPG::EnemyAction& action) const {}

        void onTroops(const std::vector<RPG::Troop>& troops) const {}
        void onTroop(const RPG::Troop& troop) const {}
        void onTroopMembers(const RPG::Troop& troop, const std::vector<RPG::TroopMember>& members) const {}
        void onTroopMember(const RPG::Troop& troop, const RPG::TroopMember& member) const {}
        void onTroopPages(const RPG::Troop& troop, const std::vector<RPG::TroopPage>& page) const {}
        void onTroopPage(const RPG::Troop& troop, const RPG::TroopPage& page) const {}
        void onTroopPageCmds(const RPG::Troop& troop, const RPG::TroopPage& page, const std::vector<RPG::EventCommand>& cmds) const {}
        //onEventCmds(cmds) called after this..
        void onTroopPageCmd(const RPG::Troop& troop, const RPG::TroopPage& page, const RPG::EventCommand& cmd) const {}
        //onEventCmd(cmd) called after this..

        void onTerrains(const std::vector<RPG::Terrain>& terrains) const {}
        void onTerrain(const RPG::Terrain& terrain) const {}

        void onAttributes(const std::vector<RPG::Attribute>& attrs) const {}
        void onAttribute(const RPG::Attribute& attr) const {}

        void onStates(const std::vector<RPG::State>& state) const {}
        void onState(const RPG::State& state) const {}

        void onAnimations(const std::vector<RPG::Animation>& anims) const {}
        void onAnimation(const RPG::Animation& anim) const {}

        void onChipsets(const std::vector<RPG::Chipset>& chipsets) const {}
        void onChipset(const RPG::Chipset& chipset) const {}

        void onBattleCommands(const RPG::BattleCommands& batcmds) const {}
        void onBattleCommandsCmds(const RPG::BattleCommands& batcmds, const std::vector<RPG::BattleCommand>& cmds) const {}
        void onBattleCommand(const RPG::BattleCommands& batcmds, const RPG::BattleCommand& batcmd) const {}

        void onClasses(const std::vector<RPG::Class>& classes) const {}
        void onClass(const RPG::Class& cls) const {}

        void onBattlerAnimations(const std::vector<RPG::BattlerAnimation>& batanims) const {}
        void onBattlerAnimation(const RPG::BattlerAnimation& batanim) const {}

        void onTerms(const RPG::Terms& terms) const {}

        void onSystem(const RPG::System& sys) const {}

        void onSwitches(const std::vector<RPG::Switch>& sws) const {}
        void onSwitch(const RPG::Switch& sw) const {}

        void onVariables(const std::vector<RPG::Variable>& vars) const {}
        void onVariable(const RPG::Variable& var) const {}

        void onTreeMap(const RPG::TreeMap& tm) const {}

        void onMapRoot(const RPG::MapInfo& map_info) const {}

        void onMap(const RPG::MapInfo& map_info, const RPG::Map& map) const {}
        void onMapEvents(const RPG::MapInfo& map_info, const RPG::Map& map, const std::vector<RPG::Event>& events) const {}
        void onMapEvent(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event) const {}
        void onMapEventPages(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const std::vector<RPG::EventPage>& pages) const {}
        void onMapEventPage(const RPG::MapInfo& map_info, const RPG::Map& map, const RPG::Event& event, const RPG::EventPage& page) const {}
        void onMapEventPageCmds(const RPG::MapInfo& map_info, const RPG::Map& map,
                const RPG::Event& event, const RPG::EventPage& page,
                const std::vector<RPG::EventCommand>& cmds) const {}
        //onEventCmds(cmds) called after this..
        void onMapEventPageCmd(const RPG::MapInfo& map_info, const RPG::Map& map,
                const RPG::Event& event, const RPG::EventPage& page,
                const RPG::EventCommand& cmd) const {}
        //onEventCmd(cmds) called after this..

        void onAreaMap(const RPG::MapInfo& map_info) const {}

        void onEventCommands(const std::vector<RPG::EventCommand>& cmds) const {}
        void onEventCommand(const RPG::EventCommand& cmds) const {}
    private:
};

//Visit the entire game.
template <typename V>
void visitRPG(V&& v) {
    v.onDatabase(Data::data);

    v.onActors(Data::actors);
    for (auto& actor: Data::actors) {
        v.onActor(actor);
    }

    v.onSkills(Data::skills);
    for (auto& skill: Data::skills) {
        v.onSkill(skill);
    }

    v.onItems(Data::items);
    for (auto& item: Data::items) {
        v.onItem(item);
    }

    v.onCommonEvents(Data::commonevents);
    for (auto& ce: Data::commonevents) {
        v.onCommonEvent(ce);
        v.onCommonEventCmds(ce, ce.event_commands);
        v.onEventCommands(ce.event_commands);
        for (auto& cmd: ce.event_commands) {
            v.onCommonEventCmd(ce, cmd);
            v.onEventCommand(cmd);
        }
    }

    v.onEnemies(Data::enemies);
    for (auto& e: Data::enemies) {
        v.onEnemy(e);
        v.onEnemyActions(e, e.actions);
        for (auto& a: e.actions) {
            v.onEnemyAction(e, a);
        }
    }

    v.onTroops(Data::troops);
    for (auto& t: Data::troops) {
        v.onTroop(t);
        v.onTroopMembers(t, t.members);
        for (auto& m: t.members) {
            v.onTroopMember(t, m);
        }
        v.onTroopPages(t, t.pages);
        for (auto& p: t.pages) {
            v.onTroopPage(t, p);
            v.onTroopPageCmds(t, p, p.event_commands);
            v.onEventCommands(p.event_commands);
            for (auto& cmd: p.event_commands) {
                v.onTroopPageCmd(t, p, cmd);
                v.onEventCommand(cmd);
            }
        }
    }

    v.onTerrains(Data::terrains);
    for (auto& t: Data::terrains) {
        v.onTerrain(t);
    }

    v.onAttributes(Data::attributes);
    for (auto& a: Data::attributes) {
        v.onAttribute(a);
    }

    v.onStates(Data::states);
    for (auto& s: Data::states) {
        v.onState(s);
    }

    v.onAnimations(Data::animations);
    for (auto& a: Data::animations) {
        v.onAnimation(a);
    }

    v.onChipsets(Data::chipsets);
    for (auto& c: Data::chipsets) {
        v.onChipset(c);
    }

    v.onBattleCommands(Data::battlecommands);
    v.onBattleCommandsCmds(Data::battlecommands, Data::battlecommands.commands);
    for (auto& c: Data::battlecommands.commands) {
        v.onBattleCommand(Data::battlecommands, c);
    }

    v.onClasses(Data::classes);
    for (auto& c: Data::classes) {
        v.onClass(c);
    }

    v.onBattlerAnimations(Data::battleranimations);
    for (auto& ba: Data::battleranimations) {
        v.onBattlerAnimation(ba);
    }

    v.onTerms(Data::terms);

    v.onSystem(Data::system);

    v.onSwitches(Data::switches);
    for (auto& sw: Data::switches) {
        v.onSwitch(sw);
    }

    v.onVariables(Data::variables);
    for (auto& var: Data::variables) {
        v.onVariable(var);
    }

    v.onTreeMap(Data::treemap);
    visitTreeMap([&](RPG::MapInfo& map_info) {
            switch(map_info.type) {
                case 0:
                    v.onMapRoot(map_info);
                    break;
                case 1: {
                    auto& map = MapCache::load(map_info);
                    v.onMap(map_info, map);
                    v.onMapEvents(map_info, map, map.events);
                    for (auto& event: map.events) {
                        v.onMapEvent(map_info, map, event);
                        v.onMapEventPages(map_info, map, event, event.pages);
                        for (auto& page: event.pages) {
                            v.onMapEventPage(map_info, map, event, page);
                            v.onMapEventPageCmds(map_info, map, event, page, page.event_commands);
                            v.onEventCommands(page.event_commands);
                            for (auto& cmd: page.event_commands) {
                                v.onMapEventPageCmd(map_info, map, event, page, cmd);
                                v.onEventCommand(cmd);
                            }
                        }
                    }
                    break;
                    }
                case 2:
                    v.onAreaMap(map_info);
                    break;
            }
        });
}

extern template void visitRPG<VisitorBase&>(VisitorBase& v);
extern template void visitRPG<const VisitorBase&>(const VisitorBase& v);
