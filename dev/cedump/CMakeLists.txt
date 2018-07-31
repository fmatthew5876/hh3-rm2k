cmake_minimum_required(VERSION 3.7)
project(cedump VERSION 1.0 LANGUAGES CXX)


add_executable(cedump
	dumpCe.C
	main.C
	)

# C++14 is required
target_compile_features(cedump PUBLIC cxx_std_14)

target_include_directories(cedump PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(cedump hh3core)
target_link_libraries(cedump liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS cedump RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
