#!/bin/sh
## update coverage git hook

set -e

export CC=gcc   CXX=g++

export BUILD_TYPE=Debug

## env variable provided at cmake configure step inside build.sh
export COPTS=\
"-D WNDX_ALGO_BUILD_TESTS=ON
 -D WNDX_ALGO_COVERAGE_ENABLE=ON
 -D WNDX_ALGO_COVERAGE_CLEAN=ON
"

echo "${BLU}build & run tests for the fresh coverage report...${END}"
./scripts/wndx_cmake_build.sh ctp
echo "${BLU}generate lcov coverage report...${END}"
./scripts/lcov.sh
echo "${BLU}update coverage in README.md...${END}"
./scripts/scov.sh

exit 0
