set(CORE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/inc/")

add_library(core SHARED
    src/geometry.cpp
    src/shapes.cpp)

target_include_directories(core PUBLIC ${CORE_HEADERS})
