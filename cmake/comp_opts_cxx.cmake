## file with CXX compiler options.
##
## vars & funcs defined and come from the funcs.cmake

## here we set flags/options common to our main target compilers
if(${GNU_COMP} OR ${Clang_COMP} OR ${AppleClang_COMP})

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
    -Wfloat-conversion

    -Wsign-promo
    -Wdouble-promotion
  )

  add_compile_options(
    -Wold-style-cast
    -Wundef
    -Wshadow
    -ftrapv # XXX: sane default or delete for the "better"?
  )

  add_compile_options(
    -fdiagnostics-color=always
    -fdiagnostics-show-template-tree
  )

  ## enable this flags depending on the situation / debugging approach
  add_compile_options(
    # -Wfatal-errors
  )
endif()

if(MSVC)
  ## TODO: mimic all other flags from the targeted compilers
  ## (to have equal warnings between compilers and all environments/platforms)
  add_compile_options(/W3)

else()
  ## ^ (flag is obviously missing in MSVC if flag has leading - sign)
  ## Other flags which may miss in any of the targeted compilers.
  ## Not targeted compilers may have support of the GNU/Clang flags
  ## -> so we check support of the following flags, applying only supported.

  ### following flags are missing in Clang

  add_check_cxx_compiler_flag( -Warith-conversion )
  add_check_cxx_compiler_flag( -Wstrict-null-sentinel )
  add_check_cxx_compiler_flag( -Wzero-as-null-pointer-constant ) # has

  ### section for the other flags (may be or may be missing in Clang)
  ### for brevity - flags for the other compilers should be here

endif()

