#pragma once
#include <exception>
#include <string>
#include <sstream>
#include "Log.H"

class Exception : public std::exception {
    public:
        explicit Exception(std::string m) : _msg(m) {}

        const char* what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
};

namespace detail {
inline void dieMsg(std::ostream&) {}

template <typename T, typename... Args>
    inline void dieMsg(std::ostream& os, T&& t, Args&&... args) {
        os << std::forward<T>(t);
        dieMsg(os, std::forward<Args>(args)...);
    }
}

template <typename... Args>
void die(Args&&... args) {
    std::ostringstream ss;
    detail::dieMsg(ss, std::forward<Args>(args)...);
    throw Exception(ss.str());
}
