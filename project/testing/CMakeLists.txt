<<<<<<< HEAD
add_executable(shapes_test shapes-test.cpp)
add_test(NAME shapes COMMAND shapes_test)

target_include_directories(shapes_test PUBLIC ${shapes_HEADERS})
target_link_libraries(shapes_test core)
target_link_libraries(shapes_test gtest)


=======
>>>>>>> 2cb5d489be76079190caa65cc88f6a6f9f5bdfca
add_executable(core_test core-test.cpp)
add_test(NAME core COMMAND core_test)

target_include_directories(core_test PUBLIC ${CORE_HEADERS})
target_link_libraries(core_test core)
target_link_libraries(core_test gtest)


<<<<<<< HEAD
=======
add_executable(shapes_test shapes-test.cpp)
add_test(NAME shapes COMMAND shapes_test)

target_include_directories(shapes_test PUBLIC ${shapes_HEADERS})
target_link_libraries(shapes_test core)
target_link_libraries(shapes_test gtest)


>>>>>>> 2cb5d489be76079190caa65cc88f6a6f9f5bdfca
add_executable(collision_test collision-test.cpp)
add_test(NAME collision COMMAND collision_test)

target_include_directories(collision_test PUBLIC ${CORE_HEADERS})
target_link_libraries(collision_test core)
target_link_libraries(collision_test gtest)


add_executable(physics_test physics-test.cpp)
add_test(NAME physics COMMAND physics_test)

target_include_directories(physics_test PUBLIC ${CORE_HEADERS})
target_include_directories(physics_test PUBLIC ${PHYSICS_HEADERS})
target_link_libraries(physics_test core)
target_link_libraries(physics_test physics)
target_link_libraries(physics_test gtest)
