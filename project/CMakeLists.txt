cmake_minimum_required(VERSION 3.16)
project(Skirmish)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED)
add_compile_options(-Wall -Werror -Wpedantic)

set(PROJECT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

option(TESTS_ENABLED "GTest modular testing enabled" ON)

if(TESTS_ENABLED)
    add_subdirectory(googletest)
    enable_testing()
    add_subdirectory(testing)
endif()


add_subdirectory(lib)