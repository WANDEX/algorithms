## useful functions / variable definitions

## define variables to be able to use short if conditions
## if(${NOT_Release})
function(not_build_type ARG)
  if(NOT CMAKE_BUILD_TYPE STREQUAL ${ARG})
    set(NOT_${ARG} TRUE PARENT_SCOPE)
  else()
    set(NOT_${ARG} FALSE PARENT_SCOPE)
  endif()
endfunction(not_build_type)
## creation of variables to be able to use them later
not_build_type(Release)
not_build_type(Debug)


## define variables to be able to use short & readable if conditions
## if(${Clang_COMP} OR ${GNU_COMP})
function(under_compiler ARG)
  if( (  CMAKE_C_COMPILER_ID STREQUAL ${ARG}) OR
      (CMAKE_CXX_COMPILER_ID STREQUAL ${ARG}) )
    if(${NOT_Release})
      message(">> UNDER ${ARG}")
    endif()
    set(${ARG}_COMP TRUE PARENT_SCOPE)
  else()
    set(${ARG}_COMP FALSE PARENT_SCOPE)
  endif()
endfunction(under_compiler)
## creation of variables to be able to use them later
under_compiler(GNU)
under_compiler(Clang)
under_compiler(AppleClang)
under_compiler(MSVC)   # cmake has short MSVC  (so this is added for consistency!)
under_compiler(cl)     # XXX: MSVC cl compiler (assuming this is somehow possible) =>
under_compiler(cl.exe) # XXX: to compile under linux -> (without big hassle with VStudio & wine)


## recursively include all sub-directories of the given dir
function(include_subdirs arg_dir)
  file(GLOB_RECURSE recurse_rpaths LIST_DIRECTORIES true
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${arg_dir}/$")
  ## ^ list only sub-directories of the given dir using relative path
  list(FILTER recurse_rpaths EXCLUDE REGEX "/\\.") # exclude paths with .dirs
  foreach(rel_path ${recurse_rpaths})
    ## get absolute path (relative paths have problems particularly in Windows)
    file(REAL_PATH "${rel_path}" abs_path BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    ## double check that this is directory
    if(NOT IS_DIRECTORY "${abs_path}")
      if(${NOT_Release})
        message("(SKIP) THIS IS NOT DIRECTORY: ${abs_path}")
      endif()
      continue()
    endif()
    ## include dir
    include_directories("${abs_path}")
    ## see which dirs are included in the build process
    if(${NOT_Release})
      message("included subdir: ${abs_path}")
    endif()
  endforeach(rel_path)
endfunction(include_subdirs)

