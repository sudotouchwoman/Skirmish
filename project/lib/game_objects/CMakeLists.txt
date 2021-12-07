set(SOURCES src/GlobalEnvironment.cpp src/GameObjects.cpp)

add_library(game_objects ${SOURCES})
target_link_libraries(game_objects PUBLIC Boost::json events plug)
target_include_directories(game_objects PUBLIC include)
target_include_directories(game_objects PUBLIC ${Boost_INCLUDE_DIRS})
