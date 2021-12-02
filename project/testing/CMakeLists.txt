add_executable(shapes_test shapes-test.cpp)
add_test(NAME shapes COMMAND shapes_test)

target_include_directories(shapes_test PUBLIC ${shapes_HEADERS})
target_link_libraries(shapes_test core)
target_link_libraries(shapes_test gtest)


add_executable(core_test core-test.cpp)
add_test(NAME core COMMAND core_test)

target_include_directories(core_test PUBLIC ${CORE_HEADERS})
target_link_libraries(core_test core)
target_link_libraries(core_test gtest)


add_executable(collision_test collision-test.cpp)
add_test(NAME collision COMMAND collision_test)

target_include_directories(collision_test PUBLIC ${CORE_HEADERS})
target_link_libraries(collision_test core)
target_link_libraries(collision_test gtest)
