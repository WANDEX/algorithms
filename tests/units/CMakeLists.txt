set(BINARY ${CMAKE_PROJECT_NAME}_units)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cc *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

target_link_libraries(${BINARY} GTest::gtest_main)

# discover tests included in the gtest binary (for ctest)
gtest_discover_tests(${BINARY})
