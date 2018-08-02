#include "Error.H"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const MapContext& ctx) {
    os << "Map: " << *ctx._map_info;
    if (ctx._event_id != 0) {
        os << " Event: `" << ctx._event_name << "' (" << ctx._event_id << ") coords=(" << ctx._event_x << ", " << ctx._event_y << ")";

        if (ctx._event_page_id != 0) {
            os << " Page: (" << ctx._event_page_id << ")";
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const CommonEventContext& ctx) {
    os << "CommonEvent: " << *ctx._ce;
    return os;
}

std::ostream& operator<<(std::ostream& os, const TroopContext& ctx) {
    os << "Troop: " << *ctx._troop;
    if (ctx._troop_page != nullptr) {
        os << " Page: (" << ctx._troop_page->ID << ")";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const ErrorContext& ctx) {
    auto& var = ctx._ctx;

    os << "ERROR: ";

    //Fixme: Better way to do this
    try {
        auto& map_ctx = std::get<MapContext>(var);
        os << map_ctx;
    } catch (...) { }
    try {
        auto& ce_ctx = std::get<CommonEventContext>(var);
        os << ce_ctx;
    } catch (...) { }
    try {
        auto& t_ctx = std::get<TroopContext>(var);
        os << t_ctx;
    } catch (...) { }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Error& e) {
    os << e._ctx << " : " << e._msg;
    return os;
}

void reportErrors(const ErrorSet& err) {
    std::cout << "Found (" << err.size() << ") errors!" << std::endl;
    for(auto& e: err) {
        std::cout << e << '\n';
    }
    std::cout << std::endl;
}
