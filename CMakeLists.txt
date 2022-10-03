cmake_minimum_required(VERSION 3.14)
project(algorithms)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
enable_testing()

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

add_subdirectory(tests/unit)
