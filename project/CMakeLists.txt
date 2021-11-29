cmake_minimum_required(VERSION 3.16)
project(Skirmish)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(PROJECT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(testing)