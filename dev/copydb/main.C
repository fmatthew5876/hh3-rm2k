#include <iostream>
#include <ldb_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"

void usage() {
    std::cerr << "copydb - Copies the RPG_RT database" << std::endl;
    std::cerr << "Usage: copydb [-d] [INFILE] [OUTFILE]" << std::endl;
}

void doMain(const std::string& infile, const std::string& outfile) {

    logInf("Loading RPG_RT database `", infile, "'...");

    std::string encoding = "";

    auto rc = LDB_Reader::Load(infile, encoding);
    if(!rc) {
        throw Exception("Failed to load LDB database from file `" + infile  + "'");
    }

    rc = LDB_Reader::Save(outfile, encoding);
    if(!rc) {
        throw Exception("Failed to save LDB database from file `" + outfile  + "'");
    }

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
    std::string outfile = argv[argc-1];

    try {
        doMain(infile, outfile);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
