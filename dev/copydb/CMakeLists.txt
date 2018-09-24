cmake_minimum_required(VERSION 3.7)
project(copydb VERSION 1.0 LANGUAGES CXX)


add_executable(copydb
	main.C
	)

# C++14 is required
target_compile_features(copydb PUBLIC cxx_std_14)

target_include_directories(copydb PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(copydb hh3core)
target_link_libraries(copydb liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS copydb RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
