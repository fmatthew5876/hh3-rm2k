#pragma once
#include <lcf/rpg/eventcommand.h>
#include "Log.H"

class CmdBuffer {
    public:
        void push_back(lcf::rpg::EventCommand cmd);

        std::vector<lcf::rpg::EventCommand> take();

        void logCmds(LogLevel ll = eLogInfo);
    private:
        std::vector<lcf::rpg::EventCommand> _cmds;
        int _indent = 0;
};
