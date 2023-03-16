cmake_minimum_required(VERSION 3.24)

## the timestamps of extracted files. NEW = time of extraction
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()

## Generate a compilation database for use with automated tools like IDE/editor
## plugins. See https://clang.llvm.org/docs/JSONCompilationDatabase.html
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(wndx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## For the list of supported compilers visit:
## https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
## Flip value to not treat warnings as errors!
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

include(cmake/funcs.cmake) # useful functions / variables
## prefer GoogleTest version installed in the system || fetch if not found
include(cmake/gtest.cmake)

include(cmake/comp_opts_cxx.cmake)

## so that the project sources can find the necessary project header files
include_subdirs(include)

add_subdirectory(tests/units)
