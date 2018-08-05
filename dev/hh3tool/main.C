#include <iostream>
#include <ldb_reader.h>
#include <lmt_reader.h>
#include <lmu_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"
#include "Args.H"
#include "Load.H"

void dispatch(const Args& args) {
}

int main(int argc, char** argv) {

    Args args;
    try {
        args = Args::load(CmdLineArgs(argv+1, argc-1));
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        Args::usage();
        return 2;
    }

    try {
        setLogLevel(args.log_level);
        loadRPG(args.load_args);

        dispatch(args);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
