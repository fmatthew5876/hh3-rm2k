#include <iostream>
#include <ldb_reader.h>
#include <lmt_reader.h>
#include <lmu_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"
#include "check.H"

void usage() {
    std::cerr << "check - Perform sanity checks on entire game" << std::endl;
    std::cerr << "Usage: check [-d] [GAMEDIR]" << std::endl;
}

void doMain(const std::string& gamedir) {

    std::string fname_ini = gamedir + "/RPG_RT.ini";
    std::string fname_ldb = gamedir + "/RPG_RT.ldb";
    std::string fname_lmt = gamedir + "/RPG_RT.lmt";

    logInf("Loading encoding from ini file `", fname_ini, "'...");

	auto encoding = ReaderUtil::GetEncoding(fname_ini);

    logInf("Loading RPG_RT database `", fname_ldb, "'...");

    auto rc = LDB_Reader::Load(fname_ldb, encoding);
    if(!rc) {
        throw Exception("Failed to load LDB database from file `" + fname_ldb  + "'");
    }

    logInf("Loading LMT tree `", fname_lmt, "'...");

    rc = LMT_Reader::Load(fname_lmt, encoding);
    if(!rc) {
        throw Exception("Failed to load LMT tree from file `" + fname_lmt  + "'");
    }

    ErrorSet err;

    checkDatabase(err);
    checkTreeMap(err, gamedir);

    reportErrors(err);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        usage();
        return 1;
    }

    if (argc > 2) {
        std::string args = argv[1];
        if (args == "-d") {
            setLogLevel(eLogDebug);
        }
    }

    std::string gamedir = argv[argc-1];

    try {
        doMain(gamedir);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
