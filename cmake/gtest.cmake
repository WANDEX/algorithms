## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
## prefer GoogleTest version installed in the system || fetch if not found
find_package(GTest 1.12.1)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    GoogleTest # release-1.12.1
    URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
  )
  FetchContent_MakeAvailable(GoogleTest)
endif()
enable_testing()
include(GoogleTest)

