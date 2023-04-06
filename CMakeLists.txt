cmake_minimum_required(VERSION 3.3)

## the timestamps of extracted files. NEW = time of extraction
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()


## ^ POLICIES
project(
  wndxlib
  VERSION       0.0.1.2
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

## target compilation options/flags
include(cmake/comp_opts_cxx.cmake)

## GUARD: check the real root of the source tree.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(WNDXLIB_BUILD_BENCHMARKS "whether or not benchmarks should be built"   OFF) # TODO: UNIMPLEMENTED
  option(WNDXLIB_BUILD_TESTS      "whether or not tests should be built"        ON)
  option(WNDXLIB_BUILD_PACKAGE    "whether or not the package should be built"  ON)
  option(WNDXLIB_COVERAGE_ENABLE  "whether or not to enable the tests coverage" ON)
  option(WNDXLIB_COVERAGE_CLEAN   "clean coverage data before taking new"       ON)
  option(WNDXLIB_MEMCHECK_ENABLE  "detect leaks via valgrind memcheck tool"     ON)
  option(WNDXLIB_INSTALL_ENABLE   "whether or not to enable the install rule"   ON)

  ## For the list of supported compilers visit:
  ## https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
  option(CMAKE_COMPILE_WARNING_AS_ERROR "treat compilation warnings as errors"  ON)

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
  endif(WNDXLIB_BUILD_PACKAGE)

  if(WNDXLIB_COVERAGE_ENABLE)
    if(${GNU_COMP})
      add_library(code_coverage INTERFACE)
      ## NOTE: without optimizations
      ## => other optimization levels/options obviously ruin coverage report.
      target_compile_options(code_coverage INTERFACE -g -O0 --coverage)
      target_link_options(code_coverage INTERFACE --coverage)

      if(WNDXLIB_COVERAGE_CLEAN)
        ### before taking new coverage analysis data:
        ## find all coverage files in the project binary dir
        file(GLOB_RECURSE prev_cov_files LIST_DIRECTORIES false
          ABSOLUTE "${PROJECT_BINARY_DIR}" *.gcda) # XXX *.gcno
        ## clean from the previous coverage data files (if any)
        if(NOT prev_cov_files STREQUAL "")
          file(REMOVE ${prev_cov_files})
          message(NOTICE ">> Cleared of previous coverage data files.")
        else()
          message(NOTICE ">> Nothing to clean up. (no coverage data files found)")
        endif()
      endif()

      message(NOTICE ">> Code Coverage will be collected!")
    else()
      message(WARNING "Code Coverage analysis enabled only on the GNU/GCC toolchain!")
    endif()
  endif(WNDXLIB_COVERAGE_ENABLE)

  if(WNDXLIB_BUILD_TESTS)
    ## prefer GoogleTest version installed in the system || fetch if not found
    include(cmake/gtest.cmake)
    add_subdirectory(tests/units)
  endif(WNDXLIB_BUILD_TESTS)

  if(WNDXLIB_MEMCHECK_ENABLE)
    ## valgrind --tool=memcheck -s --error-exitcode=73 --exit-on-first-error=yes --leak-check=full -- ./build/dev-Debug-clang/tests/units/tests_units --gtest_brief=1
    set(dpath_units "${PROJECT_BINARY_DIR}/tests/units")
    add_custom_target(memcheck
      COMMAND valgrind --tool=memcheck -s --leak-check=full --show-leak-kinds=all
                      --error-exitcode=73 --exit-on-first-error=yes
                      -- ./tests_units --gtest_brief=1
      WORKING_DIRECTORY "${dpath_units}"
      DEPENDS tests_units
    )
  endif(WNDXLIB_MEMCHECK_ENABLE)

  if(WNDXLIB_INSTALL_ENABLE)
    message(STATUS "Generating Install")
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    install(TARGETS _wndx_base wndx_core wndx_dev wndx
      EXPORT ${PROJECT_NAME}Targets
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(EXPORT ${PROJECT_NAME}Targets
      NAMESPACE   wandex::
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    configure_package_config_file(
      cmake/${PROJECT_NAME}Config.cmake.in
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
      INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )
    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    write_basic_package_version_file(
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
      COMPATIBILITY SameMinorVersion
    ) ## YYYY.MM.N1 is compatible with YYYY.MM.N2.

    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    install(DIRECTORY include/wndx # include/wndxc
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      FILES_MATCHING
      PATTERN     "*.hpp"
    )
  endif(WNDXLIB_INSTALL_ENABLE)
endif(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

