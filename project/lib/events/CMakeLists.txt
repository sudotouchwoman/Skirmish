set(SOURCES src/Serializer.cpp)

add_library(game_loop ${SOURCES})

target_link_libraries(game_loop plug game_objecst)
target_include_directories(game_loop PUBLIC include)