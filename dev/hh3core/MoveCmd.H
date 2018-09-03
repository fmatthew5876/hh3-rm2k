#pragma once
#include <vector>
#include "Exception.H"
#include "Span.H"

template <typename T, typename F>
void applyMoveCommands(T&& move_cmds, F&& f) {
    auto iter = move_cmds.begin();
    auto stcmd = iter;
    auto extend = [&](int n, std::string msg = "Corrupted move buffer!") {
        while(n > 0) {
            ++iter;
            if (iter == move_cmds.end()) {
                std::ostringstream ss;
                ss << msg << " : [";
                while(stcmd < iter) {
                    ss << *stcmd << ' ';
                    ++stcmd;
                }
                ss << "]";
                die(ss.str());
            }
            --n;
        }
    };
    //Move commands start after index 4.
    extend(3, "MoveCmd: Corrupted preamble!");
    ++iter;
    while(iter != move_cmds.end()) {
        stcmd = iter;
        if(*iter == 32 || *iter == 33) {
            //1 for switch id
            extend(1, "Corrupted Switch Cmd");
            if (*iter > 127) {
                //1 for switch id >= 128
                extend(1, "Corrupted Switch Cmd");
            }
        } else if(*iter == 34 || *iter == 35) {
            //length of filename
            extend(1, "Corrupted Graphic or SE filename length");
            int len = *iter;
            //filename chars
            extend(len, "Corrupted Graphic or SE filename text");
            if(*stcmd == 34) {
                //Sheet index
                extend(1, "Corrupted Graphic Sheet Index");
            } else {
                //sound params
                extend(3, "Corrupted Sound Params");
            }
        }
        ++iter;
        auto move_span = makeSpan(&*stcmd, iter - stcmd);
        f(move_span);
    }
}

inline int getSwitchMoveCmdSwitchId(Span<const int> cmd) {
    uint8_t prefix = cmd[1];
    if (prefix <= 127) {
        return prefix;
    }
    prefix &= 0x7F;
    uint8_t suffix = cmd[2];
    return (prefix << 7) | suffix;
}

