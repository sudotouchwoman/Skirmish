if (GTEST_LOCAL)
    find_package(GTest REQUIRED)
    set(GTEST_LINK_LIBS GTest::gtest_main GTest::gtest)
else ()
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    set(GTEST_LINK_LIBS gtest_main gtest)
endif ()

# Create binaries of tests

add_executable(game_objects-test game_objects-test.cpp)
target_link_libraries(game_objects-test ${GTEST_LINK_LIBS} game_objects)
add_test(NAME AllTestsGameObjects
        WORKING_DIRECTORY ${DATA_FILES_PATH}
        COMMAND valgrind --tool=memcheck --child-silent-after-fork=yes --leak-check=full --track-origins=yes  --show-leak-kinds=all $<TARGET_FILE:game_objects-test>)

# код кавереж
if (ENABLE_COVERAGE)
    include(CodeCoverage)

    setup_target_for_coverage_gcovr_xml(
            NAME IZ2_coverage
            EXECUTABLE ctest --verbose
            BASE_DIRECTORY ${PROJECT_FOLDER}/lib)
endif ()