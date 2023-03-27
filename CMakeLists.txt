cmake_minimum_required(VERSION 3.24)

## the timestamps of extracted files. NEW = time of extraction
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()


## ^ POLICIES
project(
  wndxlib
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

## XXX: for the wndxc lib (C headers lib - not bundled into the main project: C++ wndx lib)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


message(NOTICE "/**")
message(NOTICE " * PROJECT: ${PROJECT_NAME}")
message(NOTICE " * VERSION: ${PROJECT_VERSION}")
message(NOTICE " * DSCRPTN: ${PROJECT_DESCRIPTION}")
message(NOTICE " * HOMEURL: ${PROJECT_HOMEPAGE_URL}")
message(NOTICE " */")

include(cmake/funcs.cmake) # useful project functions / variables

## so that the sources can find the necessary header files
include_subdirs(include)

## GUARD: check the real root of the source tree.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(WNDXLIB_BUILD_BENCHMARKS "whether or not benchmarks should be built"   OFF) # TODO: UNIMPLEMENTED
  option(WNDXLIB_BUILD_TESTS      "whether or not tests should be built"        ON)
  option(WNDXLIB_BUILD_PACKAGE    "whether or not the package should be built"  ON)
  option(WNDXLIB_ENABLE_COVERAGE  "whether or not to enable the tests coverage" ON)
  option(WNDXLIB_ENABLE_INSTALL   "whether or not to enable the install rule"   ON)  # TODO: UNIMPLEMENTED

  ## For the list of supported compilers visit:
  ## https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
  option(CMAKE_COMPILE_WARNING_AS_ERROR "treat compilation warnings as errors"  ON)

  ## project compilation options/flags
  include(cmake/comp_opts_cxx.cmake)

  if(WNDXLIB_BUILD_PACKAGE)
    set(package_files include/wndx/ cmake/ CMakeLists.txt LICENSE)
    set(fpath_zip ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-src.zip)
    add_custom_command(
      OUTPUT  ${fpath_zip}
      COMMAND ${CMAKE_COMMAND} -E tar c ${fpath_zip} --format=zip -- ${package_files}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${package_files}
    )
    add_custom_target(${PROJECT_NAME}_package DEPENDS ${fpath_zip})
  endif()

  if(WNDXLIB_BUILD_TESTS)
    ## prefer GoogleTest version installed in the system || fetch if not found
    include(cmake/gtest.cmake)
    add_subdirectory(tests/units)
  endif()

endif()

