#include <iostream>
#include <ldb_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"

void dumpCommonEvent(int event_id);

void usage() {
    std::cerr << "cedump - Dump common event code" << std::endl;
    std::cerr << "Usage: cedump [-d] [FILE] [CEID]" << std::endl;
}

void doMain(const std::string& infile, int event_id) {

    logInf("Loading RPG_RT database `", infile, "'...");

    auto rc = LDB_Reader::Load(infile, "ascii");
    if(!rc) {
        throw Exception("Failed to load LDB database from file `" + infile  + "'");
    }

    dumpCommonEvent(event_id);
}

int main(int argc, char** argv) {
    if (argc <= 2) {
        usage();
        return 1;
    }

    if (argc > 3) {
        std::string args = argv[1];
        if (args == "-d") {
            setLogLevel(eLogDebug);
        }
    }

    std::string infile = argv[argc-2];
    auto event_id = atoi(argv[argc-1]);

    try {
        doMain(infile, event_id);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
