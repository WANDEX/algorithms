set(BINARY ${CMAKE_PROJECT_NAME}_tests)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cc *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} gtest)
