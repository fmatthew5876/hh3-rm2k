cmake_minimum_required(VERSION 3.7)
project(evdump VERSION 1.0 LANGUAGES CXX)


add_executable(evdump
	dumpCe.C
	dumpBat.C
	dumpMap.C
	main.C
	)

# C++14 is required
target_compile_features(evdump PUBLIC cxx_std_14)

target_include_directories(evdump PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(evdump hh3core)
target_link_libraries(evdump liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS evdump RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
