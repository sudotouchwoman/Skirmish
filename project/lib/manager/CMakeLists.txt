set(SOURCES src/manager.cpp)
add_library(manager STATIC ${SOURCES})

target_include_directories(command PUBLIC inc/)
target_link_libraries(manager command)

target_include_directories(menu PUBLIC inc/)
target_link_libraries(manager menu)
