cmake_minimum_required(VERSION 3.7)
project(hh3core VERSION 1.0 LANGUAGES CXX)


add_library(hh3core STATIC
	Log.C
	Cmd.C
	CmdBuffer.C
	MapCache.C
	Visit.C
	Load.C
	)

# C++14 is required
target_compile_features(hh3core PUBLIC cxx_std_14)

target_link_libraries(hh3core liblcf::liblcf)
target_include_directories(hh3core PUBLIC "${CMAKE_SOURCE_DIR}/ext/GSL/include/")
