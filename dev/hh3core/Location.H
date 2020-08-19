#pragma once
#include "Log.H"
#include <tuple>
#include <variant>

namespace detail {

template <typename T>
static constexpr bool isInList() {
    return false;
}

template <typename T, typename T1, typename... Args>
static constexpr bool isInList() {
    if (std::is_same<T,T1>()) {
        return true;
    }
    return isInList<T,Args...>();
}
} //namespace detail

static_assert(detail::isInList<int,int,float>(), "Pass");
static_assert(!detail::isInList<int,double,float>(), "Pass");

template <typename... Args>
class LocPath {
    public:
        LocPath(const Args*... args) : _locs(args...) {}

        template <typename... Argss>
        friend std::ostream& operator<<(std::ostream& os, const LocPath<Argss...>& loc);

        template <typename T>
            const T* getPtr() const {
                if constexpr (detail::isInList<T,Args...>()) {
                    return std::get<const T*>(_locs);
                }
                return nullptr;
            }

    private:
        std::tuple<const Args*...> _locs;
    private:
        template <size_t I> void doPrint(std::ostream& os) const;
};


using LocationVariant = std::variant<
    LocPath<>,
    LocPath<lcf::rpg::Database>,
    LocPath<lcf::rpg::Actor>,
    LocPath<lcf::rpg::Skill>,
    LocPath<lcf::rpg::Item>,
    LocPath<lcf::rpg::CommonEvent>,
    LocPath<lcf::rpg::CommonEvent,lcf::rpg::EventCommand>,
    LocPath<lcf::rpg::Enemy>,
    LocPath<lcf::rpg::Enemy,lcf::rpg::EnemyAction>,
    LocPath<lcf::rpg::Troop>,
    LocPath<lcf::rpg::Troop,lcf::rpg::TroopMember>,
    LocPath<lcf::rpg::Troop,lcf::rpg::TroopPage>,
    LocPath<lcf::rpg::Troop,lcf::rpg::TroopPage,lcf::rpg::EventCommand>,
    LocPath<lcf::rpg::Terrain>,
    LocPath<lcf::rpg::Attribute>,
    LocPath<lcf::rpg::State>,
    LocPath<lcf::rpg::Animation>,
    LocPath<lcf::rpg::Chipset>,
    LocPath<lcf::rpg::BattleCommands>,
    LocPath<lcf::rpg::BattleCommands,lcf::rpg::BattleCommand>,
    LocPath<lcf::rpg::Class>,
    LocPath<lcf::rpg::BattlerAnimation>,
    LocPath<lcf::rpg::Terms>,
    LocPath<lcf::rpg::System>,
    LocPath<lcf::rpg::Switch>,
    LocPath<lcf::rpg::Variable>,
    LocPath<lcf::rpg::TreeMap>,
    LocPath<lcf::rpg::MapInfo>,
    LocPath<lcf::rpg::MapInfo,lcf::rpg::Map>,
    LocPath<lcf::rpg::MapInfo,lcf::rpg::Map,lcf::rpg::Event>,
    LocPath<lcf::rpg::MapInfo,lcf::rpg::Map,lcf::rpg::Event,lcf::rpg::EventPage>,
    LocPath<lcf::rpg::MapInfo,lcf::rpg::Map,lcf::rpg::Event,lcf::rpg::EventPage,lcf::rpg::EventCommand>
    >;

class Location : public LocationVariant {
    public:
        using LocationVariant::LocationVariant;

        template <typename T> const T* getPtr() const {
            const LocationVariant& var = *this;
            return std::visit([](auto&& arg) {
                    return arg.template getPtr<T>();
                    }, var);
        }

        friend std::ostream& operator<<(std::ostream& os, const Location& loc);
};

template <typename T>
inline const T* getPtr(const Location& l) {
    return l.template getPtr<T>();
}



