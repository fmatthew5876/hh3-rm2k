#include <iostream>
#include <ldb_reader.h>
#include <lmt_reader.h>
#include <lmu_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"
#include "check.H"
#include "Load.H"

void usage() {
    std::cerr << "check - Perform sanity checks on entire game" << std::endl;
    std::cerr << "Usage: check [-d] [GAMEDIR]" << std::endl;
}

void doMain(const std::string& gamedir) {

    LoadArgs args;
    args.game_dir = gamedir;

    loadRPG(args);

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
