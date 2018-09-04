#include "CheckArgs.H"
#include "Exception.H"
#include <iostream>
#include <cstring>

CheckArgs CheckArgs::load(CmdLineArgs& argv) {
    CheckArgs tool_args;

    return tool_args;
}

void CheckArgs::usage() {
    std::cerr << "check [OPTS] <NAMEID> - Check the game data for bugs\n";
    std::cerr << std::endl;
}
