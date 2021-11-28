add_library(GlobalEnvironment INTERFACE)
target_link_libraries(GlobalEnvironment ${Boost_LIBRARIES})

target_include_directories(GlobalEnvironment
        INTERFACE include
        INTERFACE ${Boost_INCLUDE_DIRS})


add_library(plug INTERFACE)
target_include_directories(plug INTERFACE include)