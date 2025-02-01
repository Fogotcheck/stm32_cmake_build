cmake_minimum_required(VERSION 3.22)

project(BuildTest VERSION 0.2.3 LANGUAGES CXX C ASM)

include(FetchContent)
include(Cmake/Opts/CustomOpts.cmake)

add_subdirectory(chip)
add_subdirectory(lib)
add_subdirectory(AppMain)
