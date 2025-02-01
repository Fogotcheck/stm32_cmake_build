cmake_minimum_required(VERSION 3.22)

project(BuildTest VERSION 0.2.3 LANGUAGES CXX C ASM)

set(CPACK_PACKAGE_CHECKSUM MD5)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")

include(FetchContent)
include(CPack)
include(Cmake/Opts/CustomOpts.cmake)
include(Cmake/Utils/pre-commit.cmake)
include(Cmake/Utils/vscode-config.cmake)

add_subdirectory(chip)
add_subdirectory(lib)
add_subdirectory(AppMain)

target_install_binary(${PROJECT_NAME})
