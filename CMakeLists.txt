cmake_minimum_required(VERSION 3.14)
project(algorithms)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
enable_testing()

# so tests can find the header files
include_directories(src/c/search)
include_directories(src/cpp/sort)

add_subdirectory(tests/unit)
