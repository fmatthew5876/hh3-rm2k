#include "check.H"

void checkTroop(ErrorSet& err, const RPG::Troop& troop) {
    for(auto& page: troop.pages) {
        checkTroopPage(err, troop, page);
    }
}

void checkTroopPage(ErrorSet& err, const RPG::Troop& troop, const RPG::TroopPage& troop_page) {
    checkEventCommands(err, troop, troop_page, troop_page.event_commands);
}

