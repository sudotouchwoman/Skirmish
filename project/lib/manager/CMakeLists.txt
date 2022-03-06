set(SOURCES src/manager.cpp)
add_library(manager STATIC ${SOURCES})
target_include_directories(manager PUBLIC inc)
target_link_libraries(manager camera command fps window menu)
