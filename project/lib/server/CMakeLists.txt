set(SOURCES src/GameServer.cpp src/ConnectionServer.cpp)

add_library(server ${SOURCES})
target_include_directories(server PUBLIC include ${Boost_INCLUDE_DIRS})
target_link_libraries(server Boost::boost Threads::Threads game_loop game_objects events)