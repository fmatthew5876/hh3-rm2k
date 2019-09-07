cmake_minimum_required(VERSION 3.7)
project(hh2-dev VERSION 1.0 LANGUAGES CXX)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH APPEND "${CMAKE_CURRENT_SOURCE_DIR}/Modules")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#EasyRPG dependencies
find_package(liblcf)
find_program(LCF2XML, "lcf2xml")

#Specific tools
add_subdirectory("hh3core")
add_subdirectory("copydb")
add_subdirectory("copyrpg")
add_subdirectory("autodb")
add_subdirectory("hh3tool")

