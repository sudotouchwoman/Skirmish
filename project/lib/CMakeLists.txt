set(THREADS_PREFER_PTHREAD_FLAG ON)
set(BOOST_ROOT "/home/artem/boost")
find_package(Threads REQUIRED)
find_package(Boost)


add_subdirectory(game_objects)
add_subdirectory(game_loop)
add_subdirectory(server)
