add_library(core SHARED
    src/core.cpp
    src/entity.cpp)

target_include_directories(core PUBLIC inc/)
