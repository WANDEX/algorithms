# unit tests

file(GLOB_RECURSE units_sources LIST_DIRECTORIES false *.cc *.cpp)

add_executable(tests_units ${units_sources})
target_link_libraries(tests_units PUBLIC wandex::wndx::dev)

if(TARGET code_coverage)
  target_link_libraries(tests_units PRIVATE code_coverage)
endif()

target_link_libraries(tests_units PRIVATE GTest::gtest_main)

# discover tests included in the gtest binary (for ctest)
gtest_discover_tests(tests_units)

