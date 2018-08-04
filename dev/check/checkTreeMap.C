#include "check.H"
#include "Exception.H"
#include "Log.H"
#include "Visit.H"

#include <lmu_reader.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>

void checkMapRoot(ErrorSet& err, const RPG::MapInfo& map_info) {
    logInf("Checking MapRoot: ", map_info, " type=", map_info.type, " ... ");
}

void checkAreaMap(ErrorSet& err, const RPG::MapInfo& map_info) {
    logInf("Checking Area Map: ", map_info, " type=", map_info.type, " ... ");
}

void checkTreeMap(ErrorSet& err, const std::string& gamedir) {
    logInf("Checking TreeMap ...");

    visitTreeMap(
        [&](const RPG::MapInfo& map_info) {
            switch(map_info.type) {
                case 0:
                    checkMapRoot(err, map_info);
                    break;
                case 1:
                    {
                        std::ostringstream ss;
                        ss << gamedir << "/Map" << std::setfill('0') << std::setw(4) << map_info.ID << ".lmu";
                        auto map_path = ss.str();

                        auto map = LMU_Reader::Load(map_path, "");
                        if (!map) {
                            die("Failed to load map file `", map_path, "'");
                        }
                    checkMap(err, map_info, *map);
                }
                break;
            case 2:
                checkAreaMap(err, map_info);
                break;
            case 3:
                die("Unknown map type=", map_info.type);
                break;
        }
    });
}
