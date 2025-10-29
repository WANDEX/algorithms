#!/bin/sh
## update coverage git hook

set -e

export CC=gcc   CXX=g++

export BUILD_TYPE=Debug

build_script="./scripts/wndx_cmake_build.sh"
PRJ_NAME_UPPER=$("$build_script" --get_project_name_upper)

## env variable provided at cmake configure step inside build.sh
export COPTS=\
"-D ${PRJ_NAME_UPPER}_BUILD_TESTS=ON
 -D ${PRJ_NAME_UPPER}_COVERAGE_ENABLE=ON
 -D ${PRJ_NAME_UPPER}_COVERAGE_CLEAN=ON
"

echo "${BLU}build & run tests for the fresh coverage report...${END}"
"$build_script" ctp
echo "${BLU}generate lcov coverage report...${END}"
./scripts/lcov.sh
echo "${BLU}update coverage in README.md...${END}"
./scripts/scov.sh

exit 0
