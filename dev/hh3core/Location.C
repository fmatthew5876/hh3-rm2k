#include "Location.H"
#include "Log.H"

void locPrint(std::ostream& os, const lcf::rpg::Database& obj) {
    os << "Database: version=" << obj.version << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Actor& obj) {
    os << "Actor: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Skill& obj) {
    os << "Skill: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Item& obj) {
    os << "Item: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::CommonEvent& obj) {
    os << "CommonEvent: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Enemy& obj) {
    os << "Enemy: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::EnemyAction& obj) {
    os << "action=" << obj.ID << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Troop& obj) {
    os << "Troop: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::TroopMember& obj) {
    auto eid = obj.enemy_id;
    os << "member=" << obj.ID << " enemy=" << lcf::Data::enemies[eid-1];
}

void locPrint(std::ostream& os, const lcf::rpg::TroopPage& obj) {
    os << "page=" << obj.ID << " num_cmds=" << obj.event_commands.size() << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Terrain& obj) {
    os << "Terrain: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Attribute& obj) {
    os << "Attribute: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::State& obj) {
    os << "State: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Animation& obj) {
    os << "Animation: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Chipset& obj) {
    os << "Chipset: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::BattleCommands& obj) {
}

void locPrint(std::ostream& os, const lcf::rpg::BattleCommand& obj) {
    os << "BattleCommand: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Class& obj) {
    os << "Class: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::BattlerAnimation& obj) {
    os << "BattlerAnimation: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Terms& obj) {
    os << "Terms: ";
}

void locPrint(std::ostream& os, const lcf::rpg::System& obj) {
    os << "System: ";
}

void locPrint(std::ostream& os, const lcf::rpg::Switch& obj) {
    os << "Switch: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Variable& obj) {
    os << "Variable: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::TreeMap& obj) {
    os << "TreeMap: ";
}

void locPrint(std::ostream& os, const lcf::rpg::MapInfo& obj) {
    os << "Map: " << obj << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Map& obj) {
    os << "num_events=" << obj.events.size() << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::Event& obj) {
    os << "Event: " << obj << " coords=(" << obj.x << ", " << obj.y << ") num_pages=" << obj.pages.size() << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::EventPage& obj) {
    const char* trigger = "???";
    switch (obj.trigger) {
        case lcf::rpg::EventPage::Trigger_action:
            trigger = "Action";
            break;
        case lcf::rpg::EventPage::Trigger_touched:
            trigger = "PlayerTouch";
            break;
        case lcf::rpg::EventPage::Trigger_collision:
            trigger = "EventTouch";
            break;
        case lcf::rpg::EventPage::Trigger_auto_start:
            trigger = "AutoStart";
            break;
        case lcf::rpg::EventPage::Trigger_parallel:
            trigger = "ParallelProcess";
            break;
    }
    os << "page=" << obj.ID << " trigger=" << trigger << " num_cmds=" << obj.event_commands.size() << ' ';
}

void locPrint(std::ostream& os, const lcf::rpg::EventCommand& obj) {
    os << "Cmd: " << obj << " params=[";
    for (auto& p: obj.parameters) {
        os << p << ' ';
    }
    os << "]";
}

template <typename... Args>
template <size_t I> void LocPath<Args...>::doPrint(std::ostream& os) const {
    if constexpr(I < sizeof...(Args)) {
        auto* p = std::get<I>(_locs);
        if (p != nullptr) {
            locPrint(os, *p);
        }
        this->template doPrint<I+1>(os);
    }
}

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const LocPath<Args...>& loc) {
    loc.template doPrint<0>(os);
    return os;
}


std::ostream& operator<<(std::ostream& os, const Location& loc) {
    const LocationVariant& var = loc;
    std::visit([&os](const auto& alt) { os << alt; }, var);
    return os;
}
