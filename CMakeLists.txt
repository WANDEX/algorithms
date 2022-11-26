cmake_minimum_required(VERSION 3.24)

# the timestamps of extracted files. NEW = time of extraction
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()

# Generate a compilation database for use with automated tools like IDE/editor
# plugins. See https://clang.llvm.org/docs/JSONCompilationDatabase.html
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(algorithms)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -flto")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ftrapv") # XXX
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Wpedantic -Wall -Wextra -Wabi")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# prefer GoogleTest version installed in the system || fetch if not found
find_package(GTest 1.12.1)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    GoogleTest # release-1.12.1
    URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
    )
  FetchContent_MakeAvailable(GoogleTest)
endif()
enable_testing()
include(GoogleTest)

# recursively include all sub-directories of the given dir
function(include_subdirs arg_dir)
  file(GLOB_RECURSE recurse_rpaths LIST_DIRECTORIES true
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${arg_dir}/$")
  # ^ list only sub-directories of the given dir using relative path
  list(FILTER recurse_rpaths EXCLUDE REGEX "/\\.") # exclude paths with .dirs
  foreach(rel_path ${recurse_rpaths})
    # get absolute path (relative paths have problems particularly in Windows)
    file(REAL_PATH "${rel_path}" abs_path BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    # double check that this is directory
    if(NOT IS_DIRECTORY "${abs_path}")
      if(NOT CMAKE_BUILD_TYPE MATCHES Release)
        message("(SKIP) THIS IS NOT DIRECTORY: ${abs_path}")
      endif()
      continue()
    endif()
    # include
    include_directories("${abs_path}")
    # see which dirs are included in the build process
    if(NOT CMAKE_BUILD_TYPE MATCHES Release)
      message("included subdir: ${abs_path}")
    endif()
  endforeach(rel_path)
endfunction(include_subdirs)

# so that the project sources can find the necessary project header files
#include_subdirs(include)
include_subdirs(include/cpp)

add_subdirectory(tests/units)
