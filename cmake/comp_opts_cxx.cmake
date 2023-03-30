## file with CXX compiler options.
##
## vars & funcs defined and come from the funcs.cmake
include(cmake/funcs.cmake) # useful project functions / variables
include(GNUInstallDirs)


function(add_alias name target)
  add_library(wandex::${name} ALIAS ${target})
  set_target_properties(${target} PROPERTIES EXPORT_NAME ${name})
endfunction()


get_filename_component(wndxlib_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include REALPATH)

get_filename_component(wndxlib_include_dir_wndx  ${wndxlib_include_dir}/wndx   REALPATH)
get_filename_component(wndxlib_include_dir_wndxc ${wndxlib_include_dir}/wndxc  REALPATH)

file(GLOB_RECURSE wndxlib_headers_wndx  LIST_DIRECTORIES false ${wndxlib_include_dir_wndx}  *.hpp PARENT_SCOPE)
file(GLOB_RECURSE wndxlib_headers_wndxc LIST_DIRECTORIES false ${wndxlib_include_dir_wndxc} *.h   PARENT_SCOPE)

## recursively include all sub-directories of the given dir
file(GLOB_RECURSE wndxlib_include_dirs LIST_DIRECTORIES true ${wndxlib_include_dir}/$)
list(FILTER       wndxlib_include_dirs EXCLUDE REGEX "/\\.") # exclude paths with .dirs
## ^ to allow using short includes (for internal library development only)

## uncomment to see which dirs are included
# foreach(dir ${wndxlib_include_dirs})
#   message(${dir})
# endforeach()

## Base target for common options.
add_library(_wndx_base INTERFACE ${wndxlib_headers_wndx})
target_include_directories(_wndx_base INTERFACE
  $<BUILD_INTERFACE:${wndxlib_include_dir}>
)
target_compile_features(_wndx_base INTERFACE cxx_std_20)


## Core library.
add_library(wndx_core INTERFACE)
add_alias(wndx::core wndx_core)
target_link_libraries(wndx_core INTERFACE _wndx_base)
target_include_directories(wndx_core SYSTEM INTERFACE
  $<BUILD_INTERFACE:${wndxlib_include_dir}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


## Core library with warnings; for development only.
add_library(wndx_dev INTERFACE)
add_alias(wndx::dev wndx_dev)
target_link_libraries(wndx_dev INTERFACE _wndx_base)
target_include_directories(wndx_dev INTERFACE
  $<BUILD_INTERFACE:${wndxlib_include_dirs}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


## here we set flags/options common to our main target compilers
if(${GNU_COMP} OR ${Clang_COMP} OR ${AppleClang_COMP})

  if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_options(wndx_dev INTERFACE -O3)
  elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(wndx_dev INTERFACE -g -Og)

    ## This helps to see/fix errors (which MSVC will throw anyway)
    ## => they should be fixed. (it is crucial flag, but has its own cost)
    target_compile_options(wndx_dev INTERFACE -D_GLIBCXX_DEBUG)
  endif()

  target_compile_options(wndx_dev INTERFACE -Wall -Wextra -Wpedantic -pedantic-errors)

  ## disallow implicit conversions
  ## NOTE: with -Wconversion Clang also enables -Wsign-conversion (GCC not!)
  target_compile_options(wndx_dev INTERFACE
    -Wconversion
    -Wsign-conversion
    -Wenum-conversion
    -Wfloat-conversion

    -Wsign-promo
    -Wdouble-promotion
  )

  target_compile_options(wndx_dev INTERFACE
    -Wold-style-cast
    -Wundef
    -Wshadow
    -ftrapv # XXX: sane default or delete for the "better"?
  )

  target_compile_options(wndx_dev INTERFACE
    -fdiagnostics-color=always
    -fdiagnostics-show-template-tree
  )

  ## enable this flags depending on the situation / debugging approach
  target_compile_options(wndx_dev INTERFACE
    # -Wfatal-errors
  )
endif()

if(MSVC)
  ## TODO: mimic all other flags from the targeted compilers
  ## (to have equal warnings between compilers and all environments/platforms)
  target_compile_options(wndx_dev INTERFACE /W3)

else()
  ## ^ (flag is obviously missing in MSVC if flag has leading - sign)
  ## Other flags which may miss in any of the targeted compilers.
  ## Not targeted compilers may have support of the GNU/Clang flags
  ## -> so we check support of the following flags, applying only supported.

  ### following flags are missing in Clang

  target_add_check_cxx_compiler_flag(wndx_dev -Warith-conversion )
  target_add_check_cxx_compiler_flag(wndx_dev -Wstrict-null-sentinel )
  target_add_check_cxx_compiler_flag(wndx_dev -Wzero-as-null-pointer-constant ) # has

  ### section for the other flags (may be or may be missing in Clang)
  ### for brevity - flags for the other compilers should be here

endif()

## XXX UNUSED
## this is only for the wndxc library headers (internal use only)
add_library(wndxc INTERFACE ${wndxlib_headers_wndxc})
target_include_directories(wndxc INTERFACE
  $<BUILD_INTERFACE:${wndxlib_include_dir}>
)
target_compile_features(wndxc INTERFACE c_std_90)
## XXX

## Umbrella target with all components. (for the future buildup)
add_library(wndx INTERFACE)
add_alias(wndx wndx)
target_link_libraries(wndx INTERFACE wandex::wndx::core)

