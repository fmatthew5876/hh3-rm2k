#include "check.H"

void checkCommonEvent(ErrorSet& err, const RPG::CommonEvent& ce) {
    checkEventCommands(err, ce, ce.event_commands);
}
