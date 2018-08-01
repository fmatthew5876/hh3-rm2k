#include "check.H"
#include "Log.H"
#include <data.h>


void checkDatabase(ErrorSet& err) {
    logInf("Checking database ...");

    for (auto& ce: Data::commonevents) {
        checkCommonEvent(err, ce);
    }

    for (auto& troop: Data::troops) {
        checkTroop(err, troop);
    }
}
