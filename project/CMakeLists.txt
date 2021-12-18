set(THREADS_PREFER_PTHREAD_FLAG ON)
set(BOOST_ROOT "/home/dominik/boost")
find_package(Threads REQUIRED)
find_package(Boost 1.77.0 REQUIRED COMPONENTS json)
add_subdirectory(lib)
add_subdirectory(src)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE server)

add_executable(client_ client.cpp)
target_link_libraries(client_ PRIVATE events client)

add_executable(graphics graphics.cpp)
target_link_libraries(graphics PRIVATE manager)