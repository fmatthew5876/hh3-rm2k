cmake_minimum_required(VERSION 3.7)
project(hh3tool VERSION 1.0 LANGUAGES CXX)


add_executable(hh3tool
	GrepArgs.C
	Grep.C
	TreeMapArgs.C
	TreeMap.C
	EvDumpArgs.C
	EvDump.C
	FindItemArgs.C
	FindItem.C
	FindSwitchArgs.C
	FindSwitch.C
	FindVarArgs.C
	FindVar.C
	FindCallArgs.C
	FindCall.C
	Args.C
	main.C
	)

# C++14 is required
target_compile_features(hh3tool PUBLIC cxx_std_17)

target_include_directories(hh3tool PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(hh3tool hh3core)
target_link_libraries(hh3tool liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS hh3tool RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
