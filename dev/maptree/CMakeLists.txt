cmake_minimum_required(VERSION 3.7)
project(maptree VERSION 1.0 LANGUAGES CXX)


add_executable(maptree main.C)

# C++14 is required
target_compile_features(maptree PUBLIC cxx_std_14)

target_include_directories(maptree PUBLIC "${CMAKE_SOURCE_DIR}/hh3core")
target_link_libraries(maptree hh3core)
target_link_libraries(maptree liblcf::liblcf)

include(GNUInstallDirs)
install(TARGETS maptree RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
