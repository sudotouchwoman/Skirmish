set(GAME_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/inc/")

add_library(game SHARED
    src/object.cpp
    src/map.cpp)

target_include_directories(game PUBLIC ${GAME_HEADERS})
target_include_directories(game PRIVATE ${PHYSICS_HEADERS})
target_link_libraries(game PRIVATE physics)
