#include <iostream>
#include <ldb_reader.h>
#include <reader_util.h>

#include "Weapon.H"
#include "Log.H"
#include "Exception.H"

void usage() {
    std::cerr << "autodb - Enhances rpg_rt database with scripted elements" << std::endl;
    std::cerr << "Usage: autodb [-d] [FILE]" << std::endl;
}

void doMain(const std::string& infile) {

    logInf("Loading RPG_RT database `", infile, "'...");

    auto rc = LDB_Reader::Load(infile, "ascii");
    if(!rc) {
        throw Exception("Failed to load LDB database from file `" + infile  + "'");
    }

    doWeaponCopies();
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

    std::string infile = argv[argc-1];

    try {
        doMain(infile);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
