#include <iostream>
#include <ldb_reader.h>
#include <lmt_reader.h>
#include <lmu_reader.h>
#include <reader_util.h>
#include <data.h>

#include "Log.H"
#include "Exception.H"
#include "Load.H"
#include "MapCache.H"

void usage() {
    std::cerr << "copyrpg - Copies all files in RM/3 game." << std::endl;
    std::cerr << "Usage: copyrpg [-d] [GAMEDIR]" << std::endl;
}

void doMain(const std::string& infile, const std::string& outfile) {
    LoadArgs args;
    args.game_dir = infile;

	loadRPG(args);

#if 0
	for (auto& map_info: Data::treemap.maps) {
		if (map_info.type != RPG::TreeMap::MapType_map) {
			continue;
		}
		MapCache::loadMap(map_info);
	}
#endif

	bool rc;
	std::string out;

	for (auto& map_info: Data::treemap.maps) {
		if (map_info.type != RPG::TreeMap::MapType_map) {
			continue;
		}
		//if (map_info.ID != 995) continue;

		char buffer[256];
		snprintf(buffer, sizeof(buffer), "/Map%04d.lmu", map_info.ID);

		out = args.game_dir + buffer;

		logInf("Reading map: ", out);
		auto map = LMU_Reader::Load(out, MapCache::getEncoding());
		if (!map) {
			throw Exception("Failed to load LMU map from file `" + out + "'");
		}

		out += ".copy";

		logInf("Writing map: ", out);
		rc = LMU_Reader::Save(out, *map, MapCache::getEncoding(), SaveOpt::eNoUpdate);
		if(!rc) {
			throw Exception("Failed to save LMU map to file `" + out + "'");
		}

		//break;
	}

#if 0
	logInf("Writing maps...");
	for (auto& map_info: Data::treemap.maps) {
		if (map_info.type != RPG::TreeMap::MapType_map) {
			continue;
		}
		auto& map = MapCache::loadMap(map_info);
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "/Map%04d.lmu.copy", map_info.ID);

		out = args.game_dir + buffer;

		rc = LMU_Reader::Save(out, map, MapCache::getEncoding(), SaveOpt::eNoUpdate);
		if(!rc) {
			throw Exception("Failed to save LMU map to file `" + out + "'");
		}
	}
#endif

	out = args.game_dir + "/RPG_RT.lmt.copy";
	logInf("Writing ", out, " ...");
    rc = LMT_Reader::Save(out, MapCache::getEncoding(), SaveOpt::eNoUpdate);
    if(!rc) {
        throw Exception("Failed to save LMT treemap to file `" + out + "'");
    }

	out = args.game_dir + "/RPG_RT.ldb.copy";
	logInf("Writing ", out, " ...");
    rc = LDB_Reader::Save(out, MapCache::getEncoding(), SaveOpt::eNoUpdate);
    if(!rc) {
        throw Exception("Failed to save LDB database to file `" + out + "'");
    }

}

int main(int argc, char** argv) {

	if (argc <= 1) {
		usage();
		return -1;
	}


    if (argc > 2) {
        std::string args = argv[1];
        if (args == "-d") {
            setLogLevel(eLogDebug);
        }
    }

    std::string infile = argv[argc-1];
    std::string outfile = "";

	try {
		doMain(infile, outfile);
    } catch (const Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
