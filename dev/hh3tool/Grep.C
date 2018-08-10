#include "Visit.H"
#include "GrepArgs.H"
#include <cstring>
#include <iostream>

struct GrepVisitor : public VisitorBase {
    GrepVisitor(const GrepArgs& tool_args) : tool_args(tool_args) {}

    const GrepArgs& tool_args;

    void checkText(const char* field, const std::string& text) {
        auto& pattern = tool_args.pattern;
        bool match = false;
        if (tool_args.is_case_sensitive) {
            auto* p = strstr(text.c_str(), pattern.c_str());
            match = (p != nullptr);
        } else {
            auto* p = strcasestr(text.c_str(), pattern.c_str());
            match = (p != nullptr);
        }
        if (match) {
            std::cout << loc() << ": " << field << "=\"" << text << "\"\n";
        }
    }

#define CHECKTXT(obj, field) checkText(#field, obj.field)

    void onDatabase(const RPG::Database& db) {}

    void onActors(const std::vector<RPG::Actor>& actor) {}
    void onActor(const RPG::Actor& actor) {
        CHECKTXT(actor,name);
        CHECKTXT(actor,title);
        CHECKTXT(actor,character_name);
        CHECKTXT(actor,face_name);
    }

    void onSkills(const std::vector<RPG::Skill>& skills) {}
    void onSkill(const RPG::Skill& skill) {
        CHECKTXT(skill,name);
        CHECKTXT(skill,description);
        CHECKTXT(skill,using_message1);
        CHECKTXT(skill,using_message2);
    }

    void onItems(const std::vector<RPG::Item>& items) {}
    void onItem(const RPG::Item& item) {
        CHECKTXT(item,name);
    }

    void onCommonEvents(const std::vector<RPG::CommonEvent>& ces) {}
    void onCommonEvent(const RPG::CommonEvent& ce) {
        CHECKTXT(ce,name);
    }
    void onCommonEventCmds(const RPG::CommonEvent& ce, const std::vector<RPG::EventCommand>& cmds) {}
    //onEventCmds(cmds) called after this..
    void onCommonEventCmd(const RPG::CommonEvent& ce, const RPG::EventCommand& cmd) {}
    //onEventCmd(cmds) called after this..

    void onEnemies(const std::vector<RPG::Enemy>& enemies) {}
    void onEnemy(const RPG::Enemy& enemy) {
        CHECKTXT(enemy,name);
    }
    void onEnemyActions(const RPG::Enemy& enemy, const std::vector<RPG::EnemyAction>& actions) {}
    void onEnemyAction(const RPG::Enemy& enemy, const RPG::EnemyAction& action) {}

    void onTroops(const std::vector<RPG::Troop>& troops) {}
    void onTroop(const RPG::Troop& troop) {
        CHECKTXT(troop,name);
    }
    void onTroopMembers(const RPG::Troop& troop, const std::vector<RPG::TroopMember>& members) {}
    void onTroopMember(const RPG::Troop& troop, const RPG::TroopMember& member) {}
    void onTroopPages(const RPG::Troop& troop, const std::vector<RPG::TroopPage>& page) {}
    void onTroopPage(const RPG::Troop& troop, const RPG::TroopPage& page) {}
    void onTroopPageCmds(const RPG::Troop& troop, const RPG::TroopPage& page, const std::vector<RPG::EventCommand>& cmds) {}
    //onEventCmds(cmds) called after this..
    void onTroopPageCmd(const RPG::Troop& troop, const RPG::TroopPage& page, const RPG::EventCommand& cmd) {}
    //onEventCmd(cmd) called after this..

    void onTerrains(const std::vector<RPG::Terrain>& terrains) {}
    void onTerrain(const RPG::Terrain& terrain) {
        CHECKTXT(terrain,name);
        CHECKTXT(terrain,background_name);
    }

    void onAttributes(const std::vector<RPG::Attribute>& attrs) {}
    void onAttribute(const RPG::Attribute& attr) {
        CHECKTXT(attr,name);
    }

    void onStates(const std::vector<RPG::State>& state) {}
    void onState(const RPG::State& state) {
        CHECKTXT(state,name);
        CHECKTXT(state,message_actor);
        CHECKTXT(state,message_enemy);
        CHECKTXT(state,message_already);
        CHECKTXT(state,message_affected);
        CHECKTXT(state,message_recovery);
    }

    void onAnimations(const std::vector<RPG::Animation>& anims) {}
    void onAnimation(const RPG::Animation& anim) {
        CHECKTXT(anim,name);
        CHECKTXT(anim,animation_name);
    }

    void onChipsets(const std::vector<RPG::Chipset>& chipsets) {}
    void onChipset(const RPG::Chipset& chipset) {
        CHECKTXT(chipset,name);
        CHECKTXT(chipset,chipset_name);
    }

    void onBattleCommands(const RPG::BattleCommands& batcmds) {}
    void onBattleCommandsCmds(const RPG::BattleCommands& batcmds, const std::vector<RPG::BattleCommand>& cmds) {}
    void onBattleCommand(const RPG::BattleCommands& batcmds, const RPG::BattleCommand& batcmd) {
        CHECKTXT(batcmd,name);
    }

    void onClasses(const std::vector<RPG::Class>& classes) {}
    void onClass(const RPG::Class& cls) {}

    void onBattlerAnimations(const std::vector<RPG::BattlerAnimation>& batanims) {}
    void onBattlerAnimation(const RPG::BattlerAnimation& batanim) {}

    void onTerms(const RPG::Terms& terms) {}

    void onSystem(const RPG::System& sys) {}

    void onSwitches(const std::vector<RPG::Switch>& sws) {}
    void onSwitch(const RPG::Switch& sw) {
        CHECKTXT(sw,name);
    }

    void onVariables(const std::vector<RPG::Variable>& vars) {}
    void onVariable(const RPG::Variable& var) {
        CHECKTXT(var,name);
    }

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
    void onEventCommand(const RPG::EventCommand& cmd) {
        CHECKTXT(cmd,string);
    }
};


void doGrep(const GrepArgs& tool_args) {
    GrepVisitor v(tool_args);
    visitRPG(v);
}

void GrepArgs::doTool(const GrepArgs& tool_args) {
    doGrep(tool_args);
}
