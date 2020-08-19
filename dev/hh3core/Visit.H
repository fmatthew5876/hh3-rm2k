#pragma once
#include <lcf/data.h>
#include <lcf/rpg/map.h>
#include <algorithm>
#include "Exception.H"
#include "MapCache.H"
#include "Location.H"


class VisitStopException : public Exception {
    public:
        VisitStopException() : Exception("Visit Interrupted") {}
};


template <typename F>
void visitTreeMap(F&& f) {

    auto& maps = lcf::Data::treemap.maps;
    auto& tree_order= lcf::Data::treemap.tree_order;

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
        void onDatabase(const lcf::rpg::Database& db) const {}

        void onActors(const std::vector<lcf::rpg::Actor>& actor) const {}
        void onActor(const lcf::rpg::Actor& actor) const {}

        void onSkills(const std::vector<lcf::rpg::Skill>& skills) const {}
        void onSkill(const lcf::rpg::Skill& skill) const {}

        void onItems(const std::vector<lcf::rpg::Item>& items) const {}
        void onItem(const lcf::rpg::Item& item) const {}

        void onCommonEvents(const std::vector<lcf::rpg::CommonEvent>& ces) const {}
        void onCommonEvent(const lcf::rpg::CommonEvent& ce) const {}
        void onCommonEventCmds(const lcf::rpg::CommonEvent& ce, const std::vector<lcf::rpg::EventCommand>& cmds) const {}
        //onEventCmds(cmds) called after this..
        void onCommonEventCmd(const lcf::rpg::CommonEvent& ce, const lcf::rpg::EventCommand& cmd) const {}
        //onEventCmd(cmds) called after this..

        void onEnemies(const std::vector<lcf::rpg::Enemy>& enemies) const {}
        void onEnemy(const lcf::rpg::Enemy& enemy) const {}
        void onEnemyActions(const lcf::rpg::Enemy& enemy, const std::vector<lcf::rpg::EnemyAction>& actions) const {}
        void onEnemyAction(const lcf::rpg::Enemy& enemy, const lcf::rpg::EnemyAction& action) const {}

        void onTroops(const std::vector<lcf::rpg::Troop>& troops) const {}
        void onTroop(const lcf::rpg::Troop& troop) const {}
        void onTroopMembers(const lcf::rpg::Troop& troop, const std::vector<lcf::rpg::TroopMember>& members) const {}
        void onTroopMember(const lcf::rpg::Troop& troop, const lcf::rpg::TroopMember& member) const {}
        void onTroopPages(const lcf::rpg::Troop& troop, const std::vector<lcf::rpg::TroopPage>& page) const {}
        void onTroopPage(const lcf::rpg::Troop& troop, const lcf::rpg::TroopPage& page) const {}
        void onTroopPageCmds(const lcf::rpg::Troop& troop, const lcf::rpg::TroopPage& page, const std::vector<lcf::rpg::EventCommand>& cmds) const {}
        //onEventCmds(cmds) called after this..
        void onTroopPageCmd(const lcf::rpg::Troop& troop, const lcf::rpg::TroopPage& page, const lcf::rpg::EventCommand& cmd) const {}
        //onEventCmd(cmd) called after this..

        void onTerrains(const std::vector<lcf::rpg::Terrain>& terrains) const {}
        void onTerrain(const lcf::rpg::Terrain& terrain) const {}

        void onAttributes(const std::vector<lcf::rpg::Attribute>& attrs) const {}
        void onAttribute(const lcf::rpg::Attribute& attr) const {}

        void onStates(const std::vector<lcf::rpg::State>& state) const {}
        void onState(const lcf::rpg::State& state) const {}

        void onAnimations(const std::vector<lcf::rpg::Animation>& anims) const {}
        void onAnimation(const lcf::rpg::Animation& anim) const {}

        void onChipsets(const std::vector<lcf::rpg::Chipset>& chipsets) const {}
        void onChipset(const lcf::rpg::Chipset& chipset) const {}

        void onBattleCommands(const lcf::rpg::BattleCommands& batcmds) const {}
        void onBattleCommandsCmds(const lcf::rpg::BattleCommands& batcmds, const std::vector<lcf::rpg::BattleCommand>& cmds) const {}
        void onBattleCommand(const lcf::rpg::BattleCommands& batcmds, const lcf::rpg::BattleCommand& batcmd) const {}

        void onClasses(const std::vector<lcf::rpg::Class>& classes) const {}
        void onClass(const lcf::rpg::Class& cls) const {}

        void onBattlerAnimations(const std::vector<lcf::rpg::BattlerAnimation>& batanims) const {}
        void onBattlerAnimation(const lcf::rpg::BattlerAnimation& batanim) const {}

        void onTerms(const lcf::rpg::Terms& terms) const {}

        void onSystem(const lcf::rpg::System& sys) const {}

        void onSwitches(const std::vector<lcf::rpg::Switch>& sws) const {}
        void onSwitch(const lcf::rpg::Switch& sw) const {}

        void onVariables(const std::vector<lcf::rpg::Variable>& vars) const {}
        void onVariable(const lcf::rpg::Variable& var) const {}

        void onTreeMap(const lcf::rpg::TreeMap& tm) const {}
        void onMapInfo(const lcf::rpg::MapInfo& map_info) const {}

        void onMapRoot(const lcf::rpg::MapInfo& map_info) const {}

        void onMap(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map) const {}
        void onMapEvents(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map, const std::vector<lcf::rpg::Event>& events) const {}
        void onMapEvent(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map, const lcf::rpg::Event& event) const {}
        void onMapEventPages(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map, const lcf::rpg::Event& event, const std::vector<lcf::rpg::EventPage>& pages) const {}
        void onMapEventPage(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map, const lcf::rpg::Event& event, const lcf::rpg::EventPage& page) const {}
        void onMapEventPageCmds(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map,
                const lcf::rpg::Event& event, const lcf::rpg::EventPage& page,
                const std::vector<lcf::rpg::EventCommand>& cmds) const {}
        //onEventCmds(cmds) called after this..
        void onMapEventPageCmd(const lcf::rpg::MapInfo& map_info, const lcf::rpg::Map& map,
                const lcf::rpg::Event& event, const lcf::rpg::EventPage& page,
                const lcf::rpg::EventCommand& cmd) const {}
        //onEventCmd(cmds) called after this..

        void onAreaMap(const lcf::rpg::MapInfo& map_info) const {}

        void onEventCommands(const std::vector<lcf::rpg::EventCommand>& cmds) const {}
        void onEventCommand(const lcf::rpg::EventCommand& cmds) const {}

        Location& loc() { return _loc; }
        const Location& loc() const { return _loc; }

        void stopThisBranch() { throw VisitStopException(); }
    private:
        Location _loc;
    public:
        bool do_database = true;
        bool do_actors = true;
        bool do_skills = true;
        bool do_items = true;
        bool do_commonevents = true;
        bool do_enemies = true;
        bool do_troops = true;
        bool do_terrains = true;
        bool do_attributes = true;
        bool do_states = true;
        bool do_animations = true;
        bool do_chipsets = true;
        bool do_battlecmds = true;
        bool do_classes = true;
        bool do_battleranims = true;
        bool do_terms = true;
        bool do_system = true;
        bool do_switches = true;
        bool do_variables = true;
        bool do_maps = true;
};

//Visit the entire game.
template <typename V>
void visitRPG(V&& v) {
    if (v.do_database) {
        v.loc() = Location(LocPath(&lcf::Data::data));
        v.onDatabase(lcf::Data::data);
    }

    if (v.do_actors) {
        v.loc() = {};
        v.onActors(lcf::Data::actors);
        for (auto& actor: lcf::Data::actors) {
            v.loc() = Location(LocPath(&actor));
            v.onActor(actor);
        }
    }

    if (v.do_skills) {
        v.loc() = {};
        v.onSkills(lcf::Data::skills);
        for (auto& skill: lcf::Data::skills) {
            v.loc() = Location(LocPath(&skill));
            v.onSkill(skill);
        }
    }

    if (v.do_items) {
        v.loc() = {};
        v.onItems(lcf::Data::items);
        for (auto& item: lcf::Data::items) {
            v.loc() = Location(LocPath(&item));
            v.onItem(item);
        }
    }

    if (v.do_commonevents) {
        v.loc() = {};
        v.onCommonEvents(lcf::Data::commonevents);
        for (auto& ce: lcf::Data::commonevents) {
            v.loc() = Location(LocPath(&ce));
            v.onCommonEvent(ce);
            v.onCommonEventCmds(ce, ce.event_commands);
            v.onEventCommands(ce.event_commands);
            for (auto& cmd: ce.event_commands) {
                v.loc() = Location(LocPath(&ce, &cmd));
                v.onCommonEventCmd(ce, cmd);
                v.onEventCommand(cmd);
            }
        }
    }

    if (v.do_enemies) {
        v.loc() = {};
        v.onEnemies(lcf::Data::enemies);
        for (auto& e: lcf::Data::enemies) {
            v.loc() = Location(LocPath(&e));
            v.onEnemy(e);
            v.onEnemyActions(e, e.actions);
            for (auto& a: e.actions) {
                v.loc() = Location(LocPath(&e, &a));
                v.onEnemyAction(e, a);
            }
        }
    }

    if (v.do_troops) {
        v.loc() = {};
        v.onTroops(lcf::Data::troops);
        for (auto& t: lcf::Data::troops) {
            v.loc() = Location(LocPath(&t));
            v.onTroop(t);
            v.onTroopMembers(t, t.members);
            for (auto& m: t.members) {
                v.loc() = Location(LocPath(&t, &m));
                v.onTroopMember(t, m);
            }
            v.onTroopPages(t, t.pages);
            for (auto& p: t.pages) {
                v.loc() = Location(LocPath(&t, &p));
                v.onTroopPage(t, p);
                v.onTroopPageCmds(t, p, p.event_commands);
                v.onEventCommands(p.event_commands);
                for (auto& cmd: p.event_commands) {
                    v.loc() = Location(LocPath(&t, &p, &cmd));
                    v.onTroopPageCmd(t, p, cmd);
                    v.onEventCommand(cmd);
                }
            }
        }
    }

    if (v.do_terrains) {
        v.loc() = {};
        v.onTerrains(lcf::Data::terrains);
        for (auto& t: lcf::Data::terrains) {
            v.loc() = Location(LocPath(&t));
            v.onTerrain(t);
        }
    }

    if (v.do_attributes) {
        v.loc() = {};
        v.onAttributes(lcf::Data::attributes);
        for (auto& a: lcf::Data::attributes) {
            v.loc() = Location(LocPath(&a));
            v.onAttribute(a);
        }
    }

    if (v.do_states) {
        v.loc() = {};
        v.onStates(lcf::Data::states);
        for (auto& s: lcf::Data::states) {
            v.loc() = Location(LocPath(&s));
            v.onState(s);
        }
    }

    if (v.do_animations) {
        v.loc() = {};
        v.onAnimations(lcf::Data::animations);
        for (auto& a: lcf::Data::animations) {
            v.loc() = Location(LocPath(&a));
            v.onAnimation(a);
        }
    }

    if (v.do_chipsets) {
        v.loc() = {};
        v.onChipsets(lcf::Data::chipsets);
        for (auto& c: lcf::Data::chipsets) {
            v.loc() = Location(LocPath(&c));
            v.onChipset(c);
        }
    }

    if (v.do_battlecmds) {
        v.loc() = Location(LocPath(&lcf::Data::battlecommands));
        v.onBattleCommands(lcf::Data::battlecommands);
        v.onBattleCommandsCmds(lcf::Data::battlecommands, lcf::Data::battlecommands.commands);
        for (auto& c: lcf::Data::battlecommands.commands) {
            v.loc() = Location(LocPath(&lcf::Data::battlecommands, &c));
            v.onBattleCommand(lcf::Data::battlecommands, c);
        }
    }

    if (v.do_classes) {
        v.loc() = {};
        v.onClasses(lcf::Data::classes);
        for (auto& c: lcf::Data::classes) {
            v.loc() = Location(LocPath(&c));
            v.onClass(c);
        }
    }

    if (v.do_battleranims) {
        v.loc() = {};
        v.onBattlerAnimations(lcf::Data::battleranimations);
        for (auto& ba: lcf::Data::battleranimations) {
            v.loc() = { LocPath(&ba) };
            v.onBattlerAnimation(ba);
        }
    }

    if (v.do_terms) {
        v.loc() = { LocPath(&lcf::Data::terms) };
        v.onTerms(lcf::Data::terms);
    }

    if (v.do_system) {
        v.loc() = { LocPath(&lcf::Data::system) };
        v.onSystem(lcf::Data::system);
    }

    if (v.do_switches) {
        v.loc() = {};
        v.onSwitches(lcf::Data::switches);
        for (auto& sw: lcf::Data::switches) {
            v.loc() = { LocPath(&sw) };
            v.onSwitch(sw);
        }
    }

    if (v.do_variables) {
        v.loc() = {};
        v.onVariables(lcf::Data::variables);
        for (auto& var: lcf::Data::variables) {
            v.loc() = { LocPath(&var) };
            v.onVariable(var);
        }
    }

    if (v.do_maps) {
        v.loc() = { LocPath(&lcf::Data::treemap) };
        v.onTreeMap(lcf::Data::treemap);
        visitTreeMap([&](lcf::rpg::MapInfo& map_info) {
            v.loc() = { LocPath(&map_info) };
            v.onMapInfo(map_info);
            switch(map_info.type) {
                case lcf::rpg::TreeMap::MapType_root:
                    v.onMapRoot(map_info);
                    break;
                case lcf::rpg::TreeMap::MapType_map: {
                    auto& map = MapCache::loadMap(map_info);
                    v.loc() = { LocPath(&map_info, &map) };
                    v.onMap(map_info, map);
                    v.onMapEvents(map_info, map, map.events);
                    for (auto& event: map.events) {
                        v.loc() = { LocPath(&map_info, &map, &event) };
                        v.onMapEvent(map_info, map, event);
                        v.onMapEventPages(map_info, map, event, event.pages);
                        for (auto& page: event.pages) {
                            v.loc() = { LocPath(&map_info, &map, &event, &page) };
                            v.onMapEventPage(map_info, map, event, page);
                            v.onMapEventPageCmds(map_info, map, event, page, page.event_commands);
                            v.onEventCommands(page.event_commands);
                            for (auto& cmd: page.event_commands) {
                                v.loc() = { LocPath(&map_info, &map, &event, &page, &cmd) };
                                v.onMapEventPageCmd(map_info, map, event, page, cmd);
                                v.onEventCommand(cmd);
                            }
                        }
                    }
                    break;
                    }
                case lcf::rpg::TreeMap::MapType_area:
                    v.onAreaMap(map_info);
                    break;
            }
        });
    }
    v.loc() = {};
}

extern template void visitRPG<VisitorBase&>(VisitorBase& v);
