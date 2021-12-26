set(SOURCES src/GameLoop.cpp)

add_library(game_loop ${SOURCES})

target_link_libraries(game_loop physics game_objects settings)
target_include_directories(game_loop PUBLIC include)