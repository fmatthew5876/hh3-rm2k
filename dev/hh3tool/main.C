#include <iostream>
#include <ldb_reader.h>
#include <lmt_reader.h>
#include <lmu_reader.h>
#include <reader_util.h>

#include "Log.H"
#include "Exception.H"
#include "Args.H"
#include "Load.H"

int main(int argc, char** argv) {

    Args args;
    try {
        auto cmdline = CmdLineArgs(argv, argc).subspan(1);
        args = Args::load(cmdline);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what(), "\n");
        Args::usage();
        return 2;
    }

    try {
        setLogLevel(args.log_level);
        loadRPG(args.load_args);

        Args::dispatchTool(args);
    } catch (Exception& e) {
        logErr("Caught Exception: ", e.what());
        return 1;
    }

    return 0;
}
