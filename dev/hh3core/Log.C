#include "Log.H"
#include <iostream>

LogLevel detail::kLogLevel = eLogInfo;
std::ostream* detail::kLogStream = &std::cerr;
