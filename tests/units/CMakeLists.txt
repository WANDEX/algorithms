# unit tests

find_package(gtest REQUIRED)

enable_testing()

add_executable(tests_units main.cc)

target_sources(tests_units PRIVATE
  util/gen.t.cpp

  sort/sort.t.cpp
  str/str_trim_right.t.cc

  c/search/wndxc_binary_search.t.cc
  ds/BITreeRQPU.t.cpp
  ds/BSTuptr.t.cpp
  ds/BinaryHeapQ.t.cpp
  ds/DLLraw.t.cpp # FIXME: leaks
  ds/ListQueue.t.cpp
  ds/ListStack.t.cpp
  ds/UnionFind.t.cpp
)

target_link_libraries(tests_units PRIVATE wndx::algo::dev)

if(TARGET code_coverage)
  target_link_libraries(tests_units PRIVATE code_coverage)
endif()

target_link_libraries(tests_units PRIVATE gtest::gtest)

# enable CMake’s test runner to discover the tests included in the binary (for ctest)
include(GoogleTest)
gtest_discover_tests(tests_units
  ## explicitly state where should lay Testing/Temporary/ etc.
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

