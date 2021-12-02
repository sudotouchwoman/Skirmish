set(PHYSICS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/inc/")

add_library(physics SHARED
    src/physical.cpp
    # src/map.cpp
    src/collisions.cpp)

target_include_directories(physics PUBLIC ${PHYSICS_HEADERS})
target_include_directories(physics PUBLIC ${CORE_HEADERS})
target_link_libraries(physics PUBLIC core)
