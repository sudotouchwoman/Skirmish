cmake_minimum_required(VERSION 3.20)
project(Skirmish)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror -Wpedantic -lSDL2 -lSDL2_image)

add_subdirectory(lib/)
add_executable(main main.cpp)
target_link_libraries(main manager)