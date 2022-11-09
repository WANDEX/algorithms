cmake_minimum_required(VERSION 3.14)
project(algorithms)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# the timestamps of extracted files. NEW = time of extraction
cmake_policy(SET CMP0135 OLD)

# prefer GoogleTest version installed in the system || fetch if not found
find_package(GTest 1.12.1)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
      GoogleTest # release-1.12.1
      URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
    )
    FetchContent_MakeAvailable(GoogleTest)
endif()
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()
include(GoogleTest)

# recursively include all sub-directories of the given dir
function(include_subdirs arg_dir)
    file(GLOB_RECURSE recurse_subdirs LIST_DIRECTORIES true
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${arg_dir}/$")
    # ^ list only sub-directories of the given dir using relative path
    list(FILTER recurse_subdirs EXCLUDE REGEX "/\\.") # exclude paths with .dirs
    foreach(subdir ${recurse_subdirs})
        # uncomment to see which dirs are included in the build process
        # message("include subdir: ${subdir}")
        if (IS_DIRECTORY "${subdir}") # double check that this is directory
            include_directories("${subdir}")
        endif()
    endforeach(subdir)
endfunction(include_subdirs)

# so that the project sources can find the necessary project header files
include_subdirs(include)

add_subdirectory(tests/units)
