add_library(core SHARED
    src/core.cpp
    src/shapes.cpp)

target_include_directories(core PUBLIC inc/)
