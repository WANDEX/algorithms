cmake_minimum_required(VERSION 3.29)

## ^ POLICIES
project(
  wndx_algo
  VERSION       0.0.2
  LANGUAGES     CXX C
  DESCRIPTION   "[WIP] C++20 platform-agnostic DSA lib. (NOT FOR USE IN PRODUCTION CODEBASES)"
  HOMEPAGE_URL  "https://github.com/WANDEX/algorithms"
)

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


option(WNDX_ALGO_DISABLE_TOP_PROJECT OFF)
if(PROJECT_IS_TOP_LEVEL AND NOT WNDX_ALGO_DISABLE_TOP_PROJECT)
  set(WNDX_ALGO_IS_TOP_PROJECT ON)
else()
  set(WNDX_ALGO_IS_TOP_PROJECT OFF)
endif()

## search path for cmake modules to be loaded by: include() find_package().
## NOTE: works properly only if defined at the top-level CMakeLists.txt.
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ## project modules
  "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/wndx/sane/cmake"
)

function(targets_and_linking)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/wndx/sane")
  include(wndx_sane_modules)
  include(wndx_sane_create_targets)
  ## library targets & compilation options/flags
  wndx_sane_create_targets(PFX wndx LIB algo
    CXX_STD cxx_std_20
    PHD "include/wndx/algo"
  )

  ## inherit from wndx::sane targets
  target_link_libraries(algo_core INTERFACE wndx::sane::core) # inherit core
  target_link_libraries(algo_dev  INTERFACE wndx::sane::dev ) # inherit dev
  target_link_libraries(algo_deps    PUBLIC wndx::sane::deps) # inherit deps
  target_link_libraries(algo_src     PUBLIC wndx::sane::src ) # inherit src

  ## link sources with project dependencies from the core library target
  target_link_libraries(algo_src INTERFACE wndx::algo::core)

  ## link sources with project dependencies
  target_link_libraries(algo_src PUBLIC wndx::algo::deps)

  ## link sources with project dev interface (for the compilation flags/options)
  target_link_libraries(algo_src PRIVATE wndx::algo::dev)

  if(WNDX_ALGO_BUILD_SRC OR NOT WNDX_ALGO_IS_TOP_PROJECT)
    ## find/fetch dependecies:
    # add_subdirectory(src/wndx/algo)
  endif()
endfunction(targets_and_linking)

if(NOT WNDX_ALGO_IS_TOP_PROJECT)
  targets_and_linking()
endif()

if(WNDX_ALGO_IS_TOP_PROJECT)
  option(WNDX_ALGO_BUILD_SRC        "whether or not src should be built"          ON)
  option(WNDX_ALGO_BUILD_TESTS      "whether or not tests should be built"        OFF)
  option(WNDX_ALGO_BUILD_PACKAGE    "whether or not the package should be built"  ON)
  option(WNDX_ALGO_COVERAGE_ENABLE  "whether or not to enable the tests coverage" OFF)
  option(WNDX_ALGO_COVERAGE_CLEAN   "clean coverage data before taking new"       ON)
  option(WNDX_ALGO_INSTALL_ENABLE   "whether or not to enable the install rule"   ON)
  option(WNDX_ALGO_MEMCHECK_ENABLE  "detect leaks via valgrind memcheck tool"     OFF)
  option(WNDX_ALGO_SNTZ_ADDR        "whether or not to enable address sanitizer"  OFF) # TODO: UNIMPLEMENTED

  ## for the list of supported compilers visit:
  ## https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html
  option(CMAKE_COMPILE_WARNING_AS_ERROR "treat compilation warnings as errors" OFF)

  ## output dir is correct by default for most build setups.
  ## however, when building lib as a DLL, it is important to have the DLL in the same dir
  ## as the executable using it. Thus, we put all executables in a single /bin dir.
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  ## on macOS, property ignored for the linker import files
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

  targets_and_linking()

  if(WNDX_ALGO_COVERAGE_ENABLE)
    include(wndx_sane_coverage)
    wndx_sane_coverage(TGT_NAME code_coverage
      CLEAN WNDX_ALGO_COVERAGE_CLEAN
      RMGLOB *.gcda # *.gcno
    )
  endif(WNDX_ALGO_COVERAGE_ENABLE)

  if(WNDX_ALGO_BUILD_TESTS)
    add_subdirectory(tests/units)
  endif(WNDX_ALGO_BUILD_TESTS)

  if(WNDX_ALGO_BUILD_PACKAGE)
    include(wndx_sane_package)
    wndx_sane_package(SUFFIX src FILES
      cmake/ include/ scripts/ tests/ CMakeLists.txt LICENSE README.md
    )
  endif(WNDX_ALGO_BUILD_PACKAGE)

  if(WNDX_ALGO_MEMCHECK_ENABLE)
    include(wndx_sane_memcheck)
    wndx_sane_memcheck(TGT_NAME memcheck
      TGT_EXEC ./tests_units --gtest_brief=1
      TGT_DEPS tests_units
      # VALGRIND_OPTS --read-var-info=yes --show-leak-kinds=definite
      # EXIT_ON_FIRST_ERROR
    )
  endif(WNDX_ALGO_MEMCHECK_ENABLE)

  if(WNDX_ALGO_INSTALL_ENABLE)
    include(wndx_sane_install)
    wndx_sane_install(
      TARGETS
        # _algo_base algo_core algo_dev
        # algo_deps algo_src
        algo
      PATTERNS "*.hpp"
    )
  endif(WNDX_ALGO_INSTALL_ENABLE)
endif(WNDX_ALGO_IS_TOP_PROJECT)

