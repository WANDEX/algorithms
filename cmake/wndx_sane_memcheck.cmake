include_guard(GLOBAL)
## cmake module from WANDEX/wndx_sane lib.
##
## https://drmemory.org/page_help.html
## https://drmemory.org/page_options.html
## https://github.com/DynamoRIO/drmemory/issues/2232
## https://github.com/DynamoRIO/drmemory/issues/2487

## remove option from target. e.g. -fsanitize=address.
function(wndx_sane_tgt_rm_opt tgt opt)
  if(NOT TARGET ${tgt})
    return()
  endif()
  ## FIXME: I broke the function or why it does not remove options?
  set(fun "wndx_sane_tgt_rm_opt()")
  message(WARNING "${fun} rm opt : ${opt}")
  # set(_c_opts "") # remove from COMPILE_OPTIONS (if present)
  get_target_property(c_opts ${tgt} COMPILE_OPTIONS)
  if(_c_opts)
    list(FILTER _c_opts EXCLUDE REGEX "^${opt}$")
    set_target_properties(${tgt} PROPERTIES COMPILE_OPTIONS "${_c_opts}")
  endif(_c_opts)

  # set(l_opts "") # remove from LINK_OPTIONS (if present)
  get_target_property(_l_opts ${tgt} LINK_OPTIONS)
  if(_l_opts)
    list(FILTER _l_opts EXCLUDE REGEX "^${opt}$")
    set_target_properties(${tgt} PROPERTIES LINK_OPTIONS "${_l_opts}")
  endif(_l_opts)

  # set(i_opts "") # remove from INTERFACE_COMPILE_OPTIONS (if propagated)
  get_target_property(_i_opts ${tgt} INTERFACE_COMPILE_OPTIONS)
  if(_i_opts)
    list(FILTER _i_opts EXCLUDE REGEX "^${opt}$")
    set_target_properties(${tgt} PROPERTIES INTERFACE_COMPILE_OPTIONS "${_i_opts}")
  endif(_i_opts)
endfunction()

## helper: remove any -fsanitize=address flags from a target
# function(remove_asan_from_target tgt)
#   if (TARGET ${tgt})
#     get_target_property(_old_cflags ${tgt} COMPILE_OPTIONS)
#     if (NOT _old_cflags)
#       set(_old_cflags "")
#     endif()
#     # remove -fsanitize=address from COMPILE_OPTIONS
#     list(FILTER _old_cflags EXCLUDE REGEX "^-fsanitize=address$")
#     set_target_properties(${tgt} PROPERTIES COMPILE_OPTIONS "${_old_cflags}")

#     # remove ASAN from link flags (if present)
#     get_target_property(_old_linkopts ${tgt} LINK_OPTIONS)
#     if (NOT _old_linkopts)
#       set(_old_linkopts "")
#     endif()
#     list(FILTER _old_linkopts EXCLUDE REGEX "^-fsanitize=address$")
#     set_target_properties(${tgt} PROPERTIES LINK_OPTIONS "${_old_linkopts}")

#     # also remove from INTERFACE_COMPILE_OPTIONS if propagated
#     get_target_property(_int_opts ${tgt} INTERFACE_COMPILE_OPTIONS)
#     if(_int_opts)
#       list(FILTER _int_opts EXCLUDE REGEX "^-fsanitize=address$")
#       set_target_properties(${tgt} PROPERTIES INTERFACE_COMPILE_OPTIONS "${_int_opts}")
#     endif()
#   endif()
# endfunction()

function(wndx_sane_memcheck) ## args
  cmake_parse_arguments(arg # pfx
    "FORCE_DRMEMORY;EXIT_ON_FIRST_ERROR" # opt
    "TGT_NAME;WORKING_DIRECTORY" # ovk
    "TGT_EXEC;DRMEMORY_OPTS;LEAKS_OPTS;VALGRIND_OPTS" # mvk
    ${ARGN}
  )
  set(fun "wndx_sane_memcheck()")

  ## use default value if not explicitly provided
  if(NOT arg_WORKING_DIRECTORY OR arg_KEYWORDS_MISSING_VALUES MATCHES ".*WORKING_DIRECTORY.*")
    list(REMOVE_ITEM arg_KEYWORDS_MISSING_VALUES "WORKING_DIRECTORY")
    set(arg_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  endif()

  ## use default value if not explicitly provided
  if(NOT arg_DRMEMORY_OPTS OR arg_KEYWORDS_MISSING_VALUES MATCHES ".*DRMEMORY_OPTS.*")
    list(REMOVE_ITEM arg_KEYWORDS_MISSING_VALUES "DRMEMORY_OPTS")
    set(arg_DRMEMORY_OPTS "")
  endif()

  ## use default value if not explicitly provided
  if(NOT arg_LEAKS_OPTS OR arg_KEYWORDS_MISSING_VALUES MATCHES ".*LEAKS_OPTS.*")
    list(REMOVE_ITEM arg_KEYWORDS_MISSING_VALUES "LEAKS_OPTS")
    set(arg_LEAKS_OPTS "")
  endif()

  ## use default value if not explicitly provided
  if(NOT arg_VALGRIND_OPTS OR arg_KEYWORDS_MISSING_VALUES MATCHES ".*VALGRIND_OPTS.*")
    list(REMOVE_ITEM arg_KEYWORDS_MISSING_VALUES "VALGRIND_OPTS")
    set(arg_VALGRIND_OPTS "")
  endif()

  if(arg_UNPARSED_ARGUMENTS)
    message(WARNING "UNPARSED: ${fun} ${arg_UNPARSED_ARGUMENTS}")
  endif()
  if(arg_KEYWORDS_MISSING_VALUES)
    message(WARNING " MISSING: ${fun} ${arg_KEYWORDS_MISSING_VALUES}")
  endif()

  if(NOT arg_TGT_NAME MATCHES "^.+$")
    message(FATAL_ERROR "${fun} TGT_NAME not provided!")
  endif()
  if(NOT arg_TGT_EXEC MATCHES "^.+$")
    message(FATAL_ERROR "${fun} TGT_EXEC not provided!")
  endif()

  ## use default drmemory_suppress file located in the project root dir if file is readable.
  cmake_path(SET drmemory_suppress NORMALIZE "${PROJECT_SOURCE_DIR}/.drmemory_suppress")
  if(IS_READABLE "${drmemory_suppress}")
    list(PREPEND arg_DRMEMORY_OPTS -suppress "${drmemory_suppress}")
  endif()

  cmake_path(SET drmemory_logs_dir NORMALIZE "${arg_WORKING_DIRECTORY}/logs/drmemory")
  ## NOTE: on WIN32 -crash_at_error must be specified! => Error #1 : warning : writing to readonly memory
  ##       without this flag on WIN32 process is not terminated if leak detected!
  list(PREPEND arg_DRMEMORY_OPTS
    -batch -ignore_kernel -exit_code_if_errors 73
    -logdir "${drmemory_logs_dir}"
    -crash_at_error
  )

  list(PREPEND arg_LEAKS_OPTS
    -quiet -groupByType -conservative
  )

  list(PREPEND arg_VALGRIND_OPTS
    --tool=memcheck -s --leak-check=full --show-leak-kinds=all --error-exitcode=73
  )

  if(arg_EXIT_ON_FIRST_ERROR)
    list(APPEND arg_VALGRIND_OPTS --exit-on-first-error=yes)
    list(APPEND arg_DRMEMORY_OPTS -crash_at_error)
  endif()

  ## split on executable name and trailing arguments if any
  if(FALSE) # regex way
    set(exec_re "[A-Za-z0-9_.-]+") # regex match executable name
    string(REGEX MATCH   "${exec_re}" tgt_exec "${arg_TGT_EXEC}")
    string(REGEX REPLACE "^[^;]?${exec_re};" "" tgt_opts "${arg_TGT_EXEC}")
  else() # list way
    list(POP_FRONT  arg_TGT_EXEC tgt_exec)
    set (tgt_opts ${arg_TGT_EXEC})
  endif()
  message(DEBUG "${fun} TGT_EXEC name: ${tgt_exec}")
  message(DEBUG "${fun} TGT_EXEC opts: ${tgt_opts}")
  if(NOT TARGET ${tgt_exec})
    message(FATAL_ERROR "${fun} TGT_EXEC executable name: '${tgt_exec}' - TARGET not exist!")
  endif()

  ## NOTE: USES_TERMINAL with Ninja Generator on APPLE platform under AppleClang compiler,
  ## when leak is detected, program exec with XCODE leaks utility - hangs infinitely (horrible for CI)!
  list(APPEND CUSTOM_TARGET_OPTS
    WORKING_DIRECTORY "${arg_WORKING_DIRECTORY}"
    DEPENDS ${tgt_exec}
    VERBATIM
  )

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # target_link_libraries(${tgt_exec} PRIVATE debug -fsanitize=thread) # MutExc
    # target_link_libraries(${tgt_exec} PRIVATE debug -fsanitize=address) # XXX
    # target_link_libraries(${tgt_exec} PRIVATE debug -fsanitize=leak)
    # target_link_libraries(${tgt_exec} PRIVATE debug -fsanitize=undefined)
    ## NOTE: target_link_libraries(${tgt_exec} PRIVATE debug -fsanitize=address)
    ## CAUSES FOLLOWING ERROR on APPLE platform!
    ##
    ## Can't examine target process's malloc zone asan_0x*, so memory analysis will be incomplete or incorrect.
    ## Reason: for security, cannot load non-system library **/libclang_rt.asan_osx_dynamic.dylib
    ##
    ## Thus, we must explicitly remove from our target all MUTUALLY EXCLUSIVE OPTIONS
    ## Otherwise it will lead to undefined behavior.
    ## Because not all warnings/errors guarantee a failure with non 0 exit code.
    ## i.e. memcheck used as a CI step which is expected to FAIL if leak is detected!
    ## TODO: remove all other MUTUALLY EXCLUSIVE OPTIONS which break this main scenario!
    # XXX: WTF... now it compiles without this?
    # wndx_sane_tgt_rm_opt(${tgt_exec} -fsanitize=address)
    # remove_asan_from_target(${tgt_exec})
  endif()

  if(WIN32 OR arg_FORCE_DRMEMORY)
    find_program(DRMEMORY_COMMAND NAMES drmemory)
    if(NOT DRMEMORY_COMMAND)
      message(FATAL_ERROR "${fun} drmemory util not found at PATH!")
    else()
      message(DEBUG "${fun} drmemory util found at PATH ${DRMEMORY_COMMAND}")
    endif()
    unset(exports)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      set(exports export G_SLICE=always-malloc G_DEBUG=gc-friendly &&)
    endif() # see about: G_SLICE G_DEBUG https://drmemory.org/page_leaks.html
    unset(call)
    if(MSVC) # '$ call' makes the flow of control return to Visual Studio's batch file
      set(call call) # and hence lets other Custom Build steps run.
    endif() # https://stackoverflow.com/questions/3686837/why-are-my-custom-build-steps-not-running-in-visual-studio
    add_custom_target(${arg_TGT_NAME}
      COMMAND ${exports} ${call} ${CMAKE_COMMAND} -E make_directory "${drmemory_logs_dir}"
        && ${call} ${DRMEMORY_COMMAND} ${arg_DRMEMORY_OPTS}
        -- $<TARGET_FILE:${tgt_exec}> ${tgt_opts}
      ${CUSTOM_TARGET_OPTS}
    )
    add_dependencies(${arg_TGT_NAME} ${tgt_exec})
  elseif(APPLE)
    find_program(LEAKS_COMMAND NAMES leaks)
    if(NOT LEAKS_COMMAND)
      message(FATAL_ERROR "${fun} leaks util not found at PATH!")
    else()
      message(DEBUG "${fun} leaks util found at PATH ${LEAKS_COMMAND}")
    endif()
    ## override via $ export variable=value
    ## NOTE: cannot be used MallocScribble=${MallocScribble:-1}
    ## => Invalid character (':') in a variable name: 'MallocScribble'
    include(wndx_sane_funcs)
    set(list_def_env "")
    wndx_sane_env_set(LIST list_def_env DEF_VAL 1 ENV_VAR MallocScribble)
    wndx_sane_env_set(LIST list_def_env DEF_VAL 1 ENV_VAR MallocStackLogging)
    wndx_sane_env_set(LIST list_def_env DEF_VAL 1 ENV_VAR MallocStackLoggingNoCompact)
    add_custom_target(${arg_TGT_NAME}
      COMMAND export ${list_def_env}
        && ${LEAKS_COMMAND} ${arg_LEAKS_OPTS} -atExit
        -- $<TARGET_FILE:${tgt_exec}> ${tgt_opts}
      ${CUSTOM_TARGET_OPTS}
    )
    add_dependencies(${arg_TGT_NAME} ${tgt_exec})
  else()
    find_program(VALGRIND_COMMAND NAMES valgrind)
    if(NOT VALGRIND_COMMAND)
      message(FATAL_ERROR "${fun} valgrind util not found at PATH!")
    else()
      message(DEBUG "${fun} valgrind util found at PATH ${VALGRIND_COMMAND}")
    endif()
    add_custom_target(${arg_TGT_NAME}
      COMMAND ${VALGRIND_COMMAND} ${arg_VALGRIND_OPTS}
        -- $<TARGET_FILE:${tgt_exec}> ${tgt_opts}
      ${CUSTOM_TARGET_OPTS}
    )
    add_dependencies(${arg_TGT_NAME} ${tgt_exec})
  endif()
endfunction()