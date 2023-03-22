## file with CXX compiler options.
##
## https://cmake.org/cmake/help/latest/module/CheckCXXCompilerFlag.html
include(CheckCXXCompilerFlag) # -> check_cxx_compiler_flag

## vars defined and come from the funcs.cmake
if(${GNU_COMP} OR ${Clang_COMP} OR ${AppleClang_COMP})
  # set(CMAKE_CXX_FLAGS --coverage)

  if(CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(-O3)
  else()
    add_compile_options(-g -Og)

    ## This helps to see/fix errors (which MSVC will throw anyway)
    ## => they should be fixed. (it is crucial flag, but has its own cost)
    add_compile_options(-D_GLIBCXX_DEBUG)
  endif()

  add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors)

  ## disallow implicit conversions
  ## NOTE: with -Wconversion Clang also enables -Wsign-conversion (GCC not!)
  add_compile_options(
    -Wconversion
    -Wsign-conversion
    -Wenum-conversion
  )

  add_compile_options(
    -Wundef
    -Wshadow
    -ftrapv # XXX: sane default or delete for the "better"?
  )

  add_compile_options(-fdiagnostics-show-template-tree) # XXX ?
  add_compile_options(-fdiagnostics-color=always)

endif()

if(MSVC)
  ## TODO: mimic all other options from other compilers
  ## (to have equal varnings between compilers and all environments/platforms)
  add_compile_options(/W3)

else()
  ## other flags which may miss in any of the targeted compilers.
  ## (flag is obviously missing in MSVC if flag has leading - sign)

  ## missing in Clang
  check_cxx_compiler_flag(-Warith-conversion HAS_-Warith-conversion)
  if(HAS_-Warith-conversion)
    add_compile_options(-Warith-conversion)
  endif()

  check_cxx_compiler_flag(-Wzero-as-null-pointer-constant HAS_-Wzero-as-null-pointer-constant)
  if(HAS_-Wzero-as-null-pointer-constant)
    add_compile_options(-Wzero-as-null-pointer-constant)
  endif()

endif()

