set(THREADS_PREFER_PTHREAD_FLAG ON)
set(BOOST_ROOT "/home/artem/boost")
find_package(Threads REQUIRED)
find_package(Boost 1.77.0 REQUIRED COMPONENTS json)
add_subdirectory(lib)
add_subdirectory(src)

add_executable(server_ main.cpp)
target_link_libraries(server_ PRIVATE server)

add_executable(client client.cpp)
target_link_libraries(client PRIVATE events)
