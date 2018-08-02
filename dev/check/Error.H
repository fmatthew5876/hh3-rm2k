#pragma once
#include <data.h>
#include <variant>
#include <ostream>
#include "Log.H"

class MapContext {
    public:
        explicit MapContext(const RPG::MapInfo& mi)
            : _map_info(&mi) {}
        MapContext(const RPG::MapInfo& mi, const RPG::Event& e)
            : _map_info(&mi), _event_id(e.ID), _event_name(e.name), _event_x(e.x), _event_y(e.y) {}
        MapContext(const RPG::MapInfo& mi, const RPG::Event& e, const RPG::EventPage& ep)
            : _map_info(&mi), _event_id(e.ID), _event_name(e.name), _event_x(e.x), _event_y(e.y),_event_page_id(ep.ID) {}

        friend std::ostream& operator<<(std::ostream& os, const MapContext& ctx);

    private:
        const RPG::MapInfo* _map_info = nullptr;
        //Make copies because map goes out of scope during error processing.
        int _event_id = 0;
        std::string _event_name = "";
        int _event_x = 0;
        int _event_y = 0;
        int _event_page_id = 0;
};

struct CommonEventContext {
    public:
        explicit CommonEventContext(const RPG::CommonEvent& ce) : _ce(&ce) {}

        friend std::ostream& operator<<(std::ostream& os, const CommonEventContext& ctx);
    private:
        const RPG::CommonEvent* _ce = nullptr;
};

struct TroopContext {
    public:
        explicit TroopContext(const RPG::Troop& troop) : _troop(&troop) {}
        explicit TroopContext(const RPG::Troop& troop, const RPG::TroopPage& page)
            : _troop(&troop), _troop_page(&page) {}

        friend std::ostream& operator<<(std::ostream& os, const TroopContext& ctx);
    private:
        const RPG::Troop* _troop = nullptr;
        const RPG::TroopPage* _troop_page = nullptr;
};

class ErrorContext {
    public:
        explicit ErrorContext(MapContext ctx) : _ctx(std::move(ctx)) {}
        explicit ErrorContext(CommonEventContext ctx) : _ctx(std::move(ctx)) {}
        explicit ErrorContext(TroopContext ctx) : _ctx(std::move(ctx)) {}

        MapContext& map_ctx() { std::get<MapContext>(_ctx); }
        const MapContext& map_ctx() const { std::get<MapContext>(_ctx); }

        CommonEventContext& ce_ctx() { std::get<CommonEventContext>(_ctx); }
        const CommonEventContext& ce_ctx() const { std::get<CommonEventContext>(_ctx); }

        TroopContext& troop_ctx() { std::get<TroopContext>(_ctx); }
        const TroopContext& troop_ctx() const { std::get<TroopContext>(_ctx); }

        friend std::ostream& operator<<(std::ostream& os, const ErrorContext& ctx);
    private:
        std::variant<MapContext,CommonEventContext,TroopContext> _ctx;
};

class Error {
    public:
        template <typename... Args>
        explicit Error(ErrorContext ctx, Args&&... args)
        : _ctx(std::move(ctx)), _msg(logStr(std::forward<Args>(args)...)) { }

        friend std::ostream& operator<<(std::ostream& os, const Error& e);
    private:
        ErrorContext _ctx;
        std::string _msg;

};

using ErrorSet = std::vector<Error>;

void reportErrors(const ErrorSet& err);

