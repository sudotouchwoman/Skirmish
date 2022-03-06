find_package(SDL2 REQUIRED)
set(SOURCES src/fps.cpp)

add_library(fps ${SOURCES})
target_include_directories(fps PUBLIC inc/ ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(fps ${SDL2_IMAGE_LIBRARIES})