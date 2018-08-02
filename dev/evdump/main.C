#include <iostream>
#include <ldb_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"

void dumpCommonEvent(int event_id);
void dumpBattleEvent(int troop_id, int page);
void dumpMapEvent(const std::string& ldb_path, int map_id, int event_id, int page);

void usage() {
    std::cerr << "cedump - Dump event code" << std::endl;
    std::cerr << "Usage: evdump [-d] <LDBFILE> CE <N> - Dump Common event N code" << std::endl;
    std::cerr << "Usage: evdump [-d] <LDBFILE> BAT <T> <P> - Dump Troop T Battle Event Page P code" << std::endl;
    std::cerr << "Usage: evdump [-d] <LDBFILE> MAP <M> <E> <P> - Dump Map M Event E Page P code (include map coords)" << std::endl;
}

void doMain(const std::string& infile, int event_id) {

   dumpCommonEvent(event_id);
}

int main(int argc, char** argv) {
    if (argc <= 2) {
        usage();
        return 1;
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    auto aiter = args.begin();

    if (aiter != args.end() && *aiter == "-d") {
        setLogLevel(eLogDebug);
        ++aiter;
    }

    std::string ldb_path = "";
    enum Mode {
        eNone,
        eCE,
        eBAT,
        eMAP
    };
    Mode mode = eNone;
    int ids[3] = {};

    try {
        if (aiter == args.end()) {
            die("Invalid arguments");
        }

        ldb_path = *aiter;
        ++aiter;

        if (aiter == args.end()) {
            die("Invalid arguments");
        }

        auto& modestr = *aiter;
        ++aiter;
        for (auto& c: modestr) {
            c = std::toupper(c);
        }

        if (modestr == "CE") {
            mode = eCE;
        } else if (modestr == "BAT") {
            mode = eBAT;
        } else if (modestr == "MAP") {
            mode = eMAP;
        } else {
            die("Invalid mode `", modestr, "'");
        }


        int nids = 0;
        if(mode == eCE) {
            nids = 1;
        } else if(mode == eBAT) {
            nids = 2;
        } else if(mode == eMAP) {
            nids = 3;
        }

        for (int i = 0; i < nids; ++i) {
            if (aiter == args.end()) {
                die("Invalid arguments");
            }

            ids[i] = std::atoi(aiter->c_str());
            ++aiter;
        }

    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        usage();
        return 1;
    }

    try {
        logInf("Loading RPG_RT database `", ldb_path, "'...");

        auto encoding = "";

        auto rc = LDB_Reader::Load(ldb_path, encoding);
        if(!rc) {
            throw Exception("Failed to load LDB database from file `" + ldb_path  + "'");
        }

        switch(mode) {
            case eCE:
                dumpCommonEvent(ids[0]);
                break;
            case eBAT:
                dumpBattleEvent(ids[0], ids[1]);
                break;
            case eMAP:
                dumpMapEvent(ldb_path, ids[0], ids[1], ids[2]);
                break;
            default:
                die("???");
                break;
        }
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
