cmake_minimum_required(VERSION 3.22)

add_library(${PROJECT_NAME}_LOGGER_INTERFACE INTERFACE)

add_subdirectory(portable)
add_subdirectory(kernel)

target_link_libraries(
    ${PROJECT_NAME}_LOGGER_INTERFACE
    INTERFACE
    ${PROJECT_NAME}_LOGGER_KERNEL
    ${PROJECT_NAME}_LOGGER_PORT_INTERFACE
)
