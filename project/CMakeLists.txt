add_subdirectory(lib)

add_executable(graphics graphics.cpp)
target_link_libraries(graphics PRIVATE manager)

add_executable(graphics_1 graphics_1.cpp)
target_link_libraries(graphics_1 PRIVATE manager)