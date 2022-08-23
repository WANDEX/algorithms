cmake_minimum_required(VERSION 3.14)
project(algorithms)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
enable_testing()

# so tests can find the header files
include_directories(src/c/search)
include_directories(src/c/sort)

include_directories(src/cpp/common)
include_directories(src/cpp/datastructures/linkedlist)
include_directories(src/cpp/datastructures/stack)
include_directories(src/cpp/sort)

add_subdirectory(tests/unit)
