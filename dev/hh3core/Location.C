#include "Location.H"
#include "Log.H"

void locPrint(std::ostream& os, const RPG::Database& obj) {
    os << "Database: version=" << obj.version << ' ';
}

void locPrint(std::ostream& os, const RPG::Actor& obj) {
    os << "Actor: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Skill& obj) {
    os << "Skill: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Item& obj) {
    os << "Item: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::CommonEvent& obj) {
    os << "CommonEvent: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Enemy& obj) {
    os << "Enemy: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::EnemyAction& obj) {
    os << "action=" << obj.ID << ' ';
}

void locPrint(std::ostream& os, const RPG::Troop& obj) {
    os << "Troop: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::TroopMember& obj) {
    os << "member=" << obj.ID << ' ';
}

void locPrint(std::ostream& os, const RPG::TroopPage& obj) {
    os << "page=" << obj.ID << " num_cmds=" << obj.event_commands.size() << ' ';
}

void locPrint(std::ostream& os, const RPG::Terrain& obj) {
    os << "Terrain: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Attribute& obj) {
    os << "Attribute: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::State& obj) {
    os << "State: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Animation& obj) {
    os << "Animation: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Chipset& obj) {
    os << "Chipset: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::BattleCommands& obj) {
}

void locPrint(std::ostream& os, const RPG::BattleCommand& obj) {
    os << "BattleCommand: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Class& obj) {
    os << "Class: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::BattlerAnimation& obj) {
    os << "BattlerAnimation: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Terms& obj) {
    os << "Terms: ";
}

void locPrint(std::ostream& os, const RPG::System& obj) {
    os << "System: ";
}

void locPrint(std::ostream& os, const RPG::Switch& obj) {
    os << "Switch: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Variable& obj) {
    os << "Variable: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::TreeMap& obj) {
    os << "TreeMap: ";
}

void locPrint(std::ostream& os, const RPG::MapInfo& obj) {
    os << "Map: " << obj << ' ';
}

void locPrint(std::ostream& os, const RPG::Map& obj) {
    os << "num_events=" << obj.events.size() << ' ';
}

void locPrint(std::ostream& os, const RPG::Event& obj) {
    os << "Event: " << obj << " coords=(" << obj.x << ", " << obj.y << ") num_pages=" << obj.pages.size() << ' ';
}

void locPrint(std::ostream& os, const RPG::EventPage& obj) {
    os << "page=" << obj.ID << " num_cmds=" << obj.event_commands.size() << ' ';
}

void locPrint(std::ostream& os, const RPG::EventCommand& obj) {
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
