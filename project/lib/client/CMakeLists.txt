set(SOURCES src/ConnectionClient.cpp)

add_library(client STATIC ${SOURCES})
target_include_directories(client PUBLIC include ${Boost_INCLUDE_DIRS})
target_link_libraries(client Boost::boost Threads::Threads game_loop game_objects events settings)
target_link_libraries(client settings)
