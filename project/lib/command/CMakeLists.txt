find_package(SDL2 REQUIRED)
set(SOURCES src/command.cpp)

add_library(command STATIC ${SOURCES})
target_include_directories(command PUBLIC inc/ ${SDL2_INCLUDE_DIRS})
target_link_libraries(command ${SDL2_LIBRARIES} hero)
