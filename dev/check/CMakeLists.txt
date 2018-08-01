cmake_minimum_required(VERSION 3.7)
project(check VERSION 1.0 LANGUAGES CXX)


add_executable(check
	Error.C
	checkEventCommands.C
	checkCommonEvent.C
	checkTroop.C
	checkMapEvent.C
	checkMap.C
	checkTreeMap.C
	checkDatabase.C
	main.C
	)

# C++14 is required
target_compile_features(check PUBLIC cxx_std_17)

target_include_directories(check PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(check hh3core)
target_link_libraries(check liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS check RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
