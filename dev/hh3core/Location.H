#pragma once
#include "Log.H"
#include <tuple>
#include <variant>

template <typename... Args>
class LocPath {
    public:
        LocPath(const Args*... args) : _locs(args...) {}

        template <typename... Argss>
        friend std::ostream& operator<<(std::ostream& os, const LocPath<Argss...>& loc);

    private:
        std::tuple<const Args*...> _locs;
    private:
        template <size_t I> void doPrint(std::ostream& os) const;
};

using LocationVariant = std::variant<
    LocPath<>,
    LocPath<RPG::Database>,
    LocPath<RPG::Actor>,
    LocPath<RPG::Skill>,
    LocPath<RPG::Item>,
    LocPath<RPG::CommonEvent>,
    LocPath<RPG::CommonEvent,RPG::EventCommand>,
    LocPath<RPG::Enemy>,
    LocPath<RPG::Enemy,RPG::EnemyAction>,
    LocPath<RPG::Troop>,
    LocPath<RPG::Troop,RPG::TroopMember>,
    LocPath<RPG::Troop,RPG::TroopPage>,
    LocPath<RPG::Troop,RPG::TroopPage,RPG::EventCommand>,
    LocPath<RPG::Terrain>,
    LocPath<RPG::Attribute>,
    LocPath<RPG::State>,
    LocPath<RPG::Animation>,
    LocPath<RPG::Chipset>,
    LocPath<RPG::BattleCommands>,
    LocPath<RPG::BattleCommands,RPG::BattleCommand>,
    LocPath<RPG::Class>,
    LocPath<RPG::BattlerAnimation>,
    LocPath<RPG::Terms>,
    LocPath<RPG::System>,
    LocPath<RPG::Switch>,
    LocPath<RPG::Variable>,
    LocPath<RPG::TreeMap>,
    LocPath<RPG::MapInfo>,
    LocPath<RPG::MapInfo,RPG::Map>,
    LocPath<RPG::MapInfo,RPG::Map,RPG::Event>,
    LocPath<RPG::MapInfo,RPG::Map,RPG::Event,RPG::EventPage>,
    LocPath<RPG::MapInfo,RPG::Map,RPG::Event,RPG::EventPage,RPG::EventCommand>
    >;

class Location : public LocationVariant {
    public:
        Location() : var(LocPath<>()) {}
        using LocationVariant::LocationVariant;

        friend std::ostream& operator<<(std::ostream& os, const Location& loc);
    private:
        LocationVariant var;
};


