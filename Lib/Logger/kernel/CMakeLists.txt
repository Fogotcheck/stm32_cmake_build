cmake_minimum_required(VERSION 3.22)

set(MODULE_NAME ${PROJECT_NAME}_LOGGER_KERNEL)
add_library(${MODULE_NAME}_INTERFACE INTERFACE)

add_library(
    ${MODULE_NAME}
    src/Logger.cpp
)

target_include_directories(
    ${MODULE_NAME}_INTERFACE
    INTERFACE
    inc
    ${${PROJECT_NAME}_LOGGER_PORT_BASE_CLASS}
)

target_link_libraries(
    ${MODULE_NAME}
    ${MODULE_NAME}_INTERFACE
    freertos_kernel
    ${PROJECT_NAME}_CHIP_INTERFACE
)
