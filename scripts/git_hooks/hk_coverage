#!/bin/sh
## update coverage git hook

set -e

export CC=gcc   CXX=g++

echo "${BLU}build & run tests for the fresh coverage report...${END}"
./scripts/build.sh ctp
echo "${BLU}generate lcov coverage report...${END}"
./scripts/lcov.sh
echo "${BLU}update coverage in README.md...${END}"
./scripts/scov.sh

exit 0
