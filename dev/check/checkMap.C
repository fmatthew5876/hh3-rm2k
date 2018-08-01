#include <rpg_map.h>
#include "Log.H"
#include "Exception.H"
#include "check.H"

#include <data.h>
#include <iostream>


void checkMap(ErrorSet& err, const RPG::MapInfo& map_info, const RPG::Map& map) {
    logInf("Checking Map: ", map_info, " type=", map_info.type, " ... ");

    auto ctx = ErrorContext(MapContext{map_info});

    for(auto& event: map.events) {
        checkMapEvent(err, map, map_info, event);
    }

}
