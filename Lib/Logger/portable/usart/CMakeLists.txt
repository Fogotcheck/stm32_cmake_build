cmake_minimum_required(VERSION 3.22)

set(MODULE_NAME ${PROJECT_NAME}_USART)

add_library(${MODULE_NAME}_INTERFACE INTERFACE)

add_library(
    ${MODULE_NAME}
    src/usart.cpp
)

target_include_directories(
    ${PROJECT_NAME}_USART_INTERFACE
    INTERFACE
    inc
    ${${PROJECT_NAME}_LOGGER_PORT_BASE_CLASS}
)

target_link_libraries(
    ${MODULE_NAME}
    ${PROJECT_NAME}_USART_INTERFACE
    ${PROJECT_NAME}_CHIP_INTERFACE
)
