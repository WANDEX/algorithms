set(binary ${PROJECT_NAME}_units)

file(GLOB_RECURSE test_sources LIST_DIRECTORIES false *.cc *.cpp)

add_executable(${binary} ${test_sources})
target_link_libraries(${binary} PUBLIC wandex::wndx::dev)

if(TARGET code_coverage)
  target_link_libraries(${binary} PRIVATE code_coverage)
endif()

target_link_libraries(${binary} PRIVATE GTest::gtest_main)

# discover tests included in the gtest binary (for ctest)
gtest_discover_tests(${binary})

