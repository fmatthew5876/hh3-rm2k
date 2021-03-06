#pragma once
#include <utility>
#include <ostream>
#include <sstream>
#include <lcf/data.h>
#include <lcf/rpg/map.h>
#include <lcf/rpg/event.h>
#include <lcf/rpg/eventpage.h>
#include "Code.H"

enum LogLevel {
    eLogDebug,
    eLogInfo,
    eLogWarn,
    eLogErr
};


namespace detail {

extern LogLevel kLogLevel;
extern std::ostream* kLogStream;

inline void baselog(std::ostream& os) { }
template <typename T, typename... Args>
inline void baselog(std::ostream& os, T&& t, Args&&... args);

} //namespace detail

inline void setLogLevel(LogLevel ll) {
    detail::kLogLevel = ll;
}

inline LogLevel getLogLevel() {
    return detail::kLogLevel;
}

inline bool isLogLevelEnabled(LogLevel ll) {
    return (ll >= getLogLevel());
}

template <typename... Args>
inline void logLvlStart(LogLevel ll, Args&&... args) {
    if (isLogLevelEnabled(ll)) {
        const char* prefix = "";
        switch(ll) {
            case eLogDebug:
                prefix = "Debug: ";
                break;
            case eLogInfo:
                prefix = "Info: ";
                break;
            case eLogWarn:
                prefix = "Warn: ";
                break;
            case eLogErr:
                prefix = "Error: ";
                break;
        }
        detail::baselog(*detail::kLogStream, prefix, std::forward<Args>(args)...);
    }
}

template <typename... Args>
inline void logLvlRaw(LogLevel ll, Args&&... args) {
    if (isLogLevelEnabled(ll)) {
        detail::baselog(*detail::kLogStream, std::forward<Args>(args)...);
    }
}

template <typename... Args>
inline void logLvlEnd(LogLevel ll, Args&&... args) {
    if (isLogLevelEnabled(ll)) {
        detail::baselog(*detail::kLogStream, std::forward<Args>(args)...);
        *detail::kLogStream << std::endl;
    }
}

template <typename... Args>
inline void logLvl(LogLevel ll, Args&&... args) {
    logLvlStart(ll, std::forward<Args>(args)...);
    logLvlEnd(ll);
}

template <typename... Args>
inline void logDbg(Args&&... args) {
    logLvl(eLogDebug, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logDbgStart(Args&&... args) {
    logLvlStart(eLogDebug, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logDbgRaw(Args&&... args) {
    logLvlRaw(eLogDebug, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logDbgEnd(Args&&... args) {
    logLvlEnd(eLogDebug, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logInf(Args&&... args) {
    logLvl(eLogInfo, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logInfStart(Args&&... args) {
    logLvlStart(eLogInfo, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logInfRaw(Args&&... args) {
    logLvlRaw(eLogInfo, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logInfEnd(Args&&... args) {
    logLvlEnd(eLogInfo, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logWrn(Args&&... args) {
    logLvl(eLogWarn, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logWrnStart(Args&&... args) {
    logLvlStart(eLogWarn, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logWrnRaw(Args&&... args) {
    logLvlRaw(eLogWarn, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logWrnEnd(Args&&... args) {
    logLvlEnd(eLogWarn, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logErr(Args&&... args) {
    logLvl(eLogErr, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logErrStart(Args&&... args) {
    logLvlStart(eLogErr, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logErrRaw(Args&&... args) {
    logLvlRaw(eLogErr, std::forward<Args>(args)...);
}

template <typename... Args>
inline void logErrEnd(Args&&... args) {
    logLvlEnd(eLogErr, std::forward<Args>(args)...);
}

template <typename... Args>
inline std::string logStr(Args&&... args) {
    std::ostringstream ss;
    detail::baselog(ss, std::forward<Args>(args)...);
    return ss.str();
}

namespace RPG {
class Actor;
class Skill;
class Item;
class Troop;
class Terrain;
class Attribute;
class State;
class Animation;
class Chipset;
class CommonEvent;
class Switch;
class Variable;
};

namespace detail {

template <typename T>
inline std::ostream& printRpg(std::ostream& os, T&& t) {
    os << '`' << t.name << "' (" << t.ID << ')';
	return os;
}
};

#if 0
inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Actor& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Skill& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Item& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Enemy& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Troop& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Terrain& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Attribute& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::State& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Animation& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Chipset& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::CommonEvent& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Switch& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Variable& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Event& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::MapInfo& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::BattleCommand& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::Class& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::BattlerAnimation& t) {
    return detail::printRpg(os, t);
}

inline std::ostream& operator<<(std::ostream& os, const lcf::rpg::EventCommand& t) {
    os << '`' << codeToStr(lcf::rpg::EventCommand::Code(t.code)) << "'(" << t.code << ')';
    return os;
}
#endif

template <typename T, typename... Args>
inline void detail::baselog(std::ostream& os, T&& t, Args&&... args) {
    os << std::forward<T>(t);
    baselog(os, std::forward<Args>(args)...);
}

