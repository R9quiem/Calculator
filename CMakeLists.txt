cmake_minimum_required(VERSION 3.21)

project(calc LANGUAGES CXX)

# Укажем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plugin_api INTERFACE)

target_include_directories(plugin_api INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)


# Исполняемый файл проекта
add_executable(calc
    app/main.cpp
)

target_link_libraries(calc PRIVATE plugin_api)

target_include_directories(calc PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/plugins)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)