cmake_minimum_required(VERSION 3.7)
project(autodb VERSION 1.0 LANGUAGES CXX)


add_executable(autodb
	Weapon.C
	main.C
	)

# C++14 is required
target_compile_features(autodb PUBLIC cxx_std_14)

target_include_directories(autodb PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(autodb hh3core)
target_link_libraries(autodb liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS autodb RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
