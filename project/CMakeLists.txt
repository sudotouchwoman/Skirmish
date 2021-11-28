add_subdirectory(lib)
#add_subdirectory(src)
add_executable(aboba main.cpp)
target_link_libraries(aboba server)
