enable_testing()

add_executable(core_test core-test.cpp)
add_test(NAME core COMMAND core_test)

target_include_directories(core_test PUBLIC "${PROJECT_FOLDER}/lib/core/inc/")
target_link_libraries(core_test core)