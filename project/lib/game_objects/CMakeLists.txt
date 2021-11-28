set(SOURCES src/GlobalEnvironment.cpp)

add_library(game_objects ${SOURCES})
target_link_libraries(game_objects PRIVATE ${Boost_LIBRARIES})
target_include_directories(game_objects PUBLIC include ${Boost_INCLUDE_DIRS})
