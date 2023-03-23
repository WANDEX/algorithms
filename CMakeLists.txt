cmake_minimum_required(VERSION 3.24)

## the timestamps of extracted files. NEW = time of extraction
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()


## ^ POLICIES
project(
  wndx
  VERSION       0.0.1.0
  LANGUAGES     CXX C
  DESCRIPTION   "[WIP] C++20 platform-agnostic DSA lib. (NOT FOR USE IN PRODUCTION CODEBASES)"
  HOMEPAGE_URL  "https://github.com/WANDEX/algorithms"
)

## C++23: uz size_t suffix literal
## (P0330R8) but MSVC does not support it yet. :(
## https://en.cppreference.com/w/cpp/compiler_support#C.2B.2B23_features
# set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## XXX: for the wndxc lib (currently unused)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

## Generate a compilation database for use with automated tools like IDE/editor
## plugins. See https://clang.llvm.org/docs/JSONCompilationDatabase.html
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## For the list of supported compilers visit:
## https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
## NOTE: Flip value to not treat warnings as errors!
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

message("/**")
message(" * PROJECT: ${PROJECT_NAME}")
message(" * VERSION: ${PROJECT_VERSION}")
message(" * DSCRPTN: ${PROJECT_DESCRIPTION}")
message(" * HOMEURL: ${PROJECT_HOMEPAGE_URL}")
message(" */")

include(cmake/funcs.cmake) # useful functions / variables
## prefer GoogleTest version installed in the system || fetch if not found
include(cmake/gtest.cmake)

include(cmake/comp_opts_cxx.cmake)

# TODO: add via option or smth...
# set(CMAKE_CXX_FLAGS --coverage)

## so that the project sources can find the necessary project header files
include_subdirs(include)

add_subdirectory(tests/units)

