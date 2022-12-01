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

include(cmake/funcs.cmake) # useful functions / variables
# prefer GoogleTest version installed in the system || fetch if not found
include(cmake/gtest.cmake)

if(${Clang_COMP} OR ${GNU_COMP})
  add_compile_options(-g -O0)
  add_compile_options(-ftrapv) # XXX
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(MSVC OR ${cl_COMP})
  # NOTE: /WX - should be added by developer (all warnings should be fixed!)
  add_compile_options(/Wall /WL /sdl)
endif()

# so that the project sources can find the necessary project header files
include_subdirs(include)

add_subdirectory(tests/units)
