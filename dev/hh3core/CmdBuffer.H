#pragma once
#include <rpg_eventcommand.h>
#include "Log.H"

class CmdBuffer {
    public:
        void push_back(RPG::EventCommand cmd);

        std::vector<RPG::EventCommand> take();

        void logCmds(LogLevel ll = eLogInfo);
    private:
        std::vector<RPG::EventCommand> _cmds;
        int _indent = 0;
};
