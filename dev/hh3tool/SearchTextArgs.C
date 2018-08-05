#include "SearchTextArgs.H"
#include "Exception.H"

SearchTextArgs SearchTextArgs::load(CmdLineArgs argv) {
    SearchTextArgs tool_args;

    if (argv.empty()) {
        throw Exception("No search string specified!");
    }

    tool_args.text = argv[0];
    argv = argv.subspan(1);

    return tool_args;
}

void SearchTextArgs::usage() {
}
