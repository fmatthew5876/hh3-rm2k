#include "Load.H"
#include <lcf/ldb/reader.h>
#include <lcf/lmt/reader.h>
#include <lcf/reader_util.h>
#include "Log.H"
#include "Exception.H"
#include "MapCache.H"

static void inferFilesFromGameDir(LoadArgs& args) {
    if (args.game_dir.empty()) {
        return;
    }
    if (args.ini_path.empty()) {
        args.ini_path = args.game_dir + "/RPG_RT.ini";
    }
    if (args.ldb_path.empty()) {
        args.ldb_path = args.game_dir + "/RPG_RT.ldb";
    }
    if (args.lmt_path.empty()) {
        args.lmt_path = args.game_dir + "/RPG_RT.lmt";
    }
    if (args.map_dir.empty()) {
        args.map_dir = args.game_dir;
    }
}

void loadRPG(const LoadArgs& in_args) {

    auto args = in_args;

    inferFilesFromGameDir(args);
    auto encoding = std::string();

    if (!args.ini_path.empty()) {
        logInf("Loading encoding from ini file `", args.ini_path, "' ...");

        //CHeck if file exists

        encoding = lcf::ReaderUtil::GetEncoding(args.ini_path);
    }
    MapCache::setEncoding(encoding);

    if (!args.ldb_path.empty()) {
        logInf("Loading RPG_RT database `", args.ldb_path, "' [", encoding, "] ...");

        auto rc = lcf::LDB_Reader::Load(args.ldb_path, encoding);
        if(!rc) {
            die("Failed to load LDB database from file `" + args.ldb_path  + "'");
        }
    }

    if(!args.lmt_path.empty()) {
        logInf("Loading LMT tree `", args.lmt_path, "' [", encoding, "] ...");

        auto rc = lcf::LMT_Reader::Load(args.lmt_path, encoding);
        if(!rc) {
            throw Exception("Failed to load LMT tree from file `" + args.lmt_path  + "'");
        }
    }

    if(!args.map_dir.empty()) {
        logInf("Configuring system for map directory `", args.map_dir, "'...");

        MapCache::setMapDir(args.map_dir);
    }
}
