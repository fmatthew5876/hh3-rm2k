#include "check.H"

void checkMapEventPage(ErrorSet& err, const RPG::Map& map, const RPG::MapInfo& map_info, const RPG::Event& event, const RPG::EventPage& event_page) {
    checkEventCommands(err, map, map_info, event, event_page, event_page.event_commands);
}

void checkMapEvent(ErrorSet& err, const RPG::Map& map, const RPG::MapInfo& map_info, const RPG::Event& event) {
    auto ctx = ErrorContext(MapContext{map_info, event});

    for (auto& page: event.pages) {
        checkMapEventPage(err, map, map_info, event, page);
    }
}


