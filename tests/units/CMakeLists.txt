# unit tests

enable_testing()

file(GLOB_RECURSE units_sources LIST_DIRECTORIES false *.cc *.cpp)

add_executable(tests_units ${units_sources})
target_link_libraries(tests_units PRIVATE wndx::algo::dev)

if(TARGET code_coverage)
  target_link_libraries(tests_units PRIVATE code_coverage)
endif()

target_link_libraries(tests_units PRIVATE gtest::gtest_main)

# enable CMake’s test runner to discover the tests included in the binary (for ctest)
include(GoogleTest)
gtest_discover_tests(tests_units)

