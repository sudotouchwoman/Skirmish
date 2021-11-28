set(SOURCES #[[src/Connection.cpp]] src/GameServer.cpp src/ConnectionServer.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost)

add_library(server ${SOURCES})
target_include_directories(server PUBLIC include ${Boost_INCLUDE_DIRS})
target_link_libraries(server ${Boost_LIBRARIES} Threads::Threads #[[GameLoop]])