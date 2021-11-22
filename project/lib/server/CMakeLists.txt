set(SOURCES src/Connection.cpp src/Server.cpp)

find_package(Boost)

add_library(server ${SOURCES})
target_include_directories(server PUBLIC include ${Boost_INCLUDE_DIRS})
target_link_libraries(server ${Boost_LIBRARIES})