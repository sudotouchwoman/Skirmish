set(SOURCES src/camera.cpp)

add_library(camera STATIC ${SOURCES})
target_include_directories(camera PUBLIC inc/)
target_link_libraries(camera window game_objects settings)