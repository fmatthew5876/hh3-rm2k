cmake_minimum_required(VERSION 3.7)
project(copyrpg VERSION 1.0 LANGUAGES CXX)


add_executable(copyrpg
	main.C
	)

# C++14 is required
target_compile_features(copyrpg PUBLIC cxx_std_17)

target_include_directories(copyrpg PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(copyrpg hh3core)
target_link_libraries(copyrpg liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS copyrpg RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
