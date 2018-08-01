cmake_minimum_required(VERSION 3.7)
project(hh2-dev VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH APPEND "${CMAKE_CURRENT_SOURCE_DIR}/Modules")

#EasyRPG dependencies
find_package(liblcf)
find_program(LCF2XML, "lcf2xml")

#Specific tools
add_subdirectory("hh3core")
add_subdirectory("autodb")
add_subdirectory("evdump")
add_subdirectory("check")

