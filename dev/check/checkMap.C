#include <rpg_map.h>
#include "Log.H"
#include "Exception.H"
#include "check.H"

#include <data.h>
#include <iostream>

void checkMapBGMLegacyWavTempo(ErrorSet& err, ErrorContext& ctx, const RPG::MapInfo& map_info) {
    auto& music = map_info.music;

    if (music.tempo != 100 && music.name.substr(0, 2) != "SE") {
        err.push_back(Error(ctx, "Map has BGM ", music.name, " with tempo ", music.tempo, "!"));
    }
}

void checkMap(ErrorSet& err, const RPG::MapInfo& map_info, const RPG::Map& map) {
    logInf("Checking Map: ", map_info, " type=", map_info.type, " ... ");

    auto ctx = ErrorContext(MapContext{map_info});

    checkMapBGMLegacyWavTempo(err, ctx, map_info);

    for(auto& event: map.events) {
        checkMapEvent(err, map, map_info, event);
    }

}
