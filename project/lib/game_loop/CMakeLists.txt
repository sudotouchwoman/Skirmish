set(SOURCES src/GameLoop.cpp)

add_library(GameLoop ${SOURCES})

target_link_libraries(GameLoop GlobalEnvironment plug)
target_include_directories(GameLoop PUBLIC include)