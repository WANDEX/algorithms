#!/bin/sh
# build & optionally run tests
#
## make clean build & run tests:
# ./scripts/build.sh clean tests
## build & run tests:
# ./scripts/build.sh tests
## just build:
# ./scripts/build.sh
## build & run tests filtered by --gtest_filter:
# ./scripts/build.sh tests *filter*

bdir="build"
NOT_USE_MAKE_C_DIR=1 # NOTE: comment out if you prefer to use make -C option

set -e

if [ -r ./CMakeLists.txt ]; then
  _="" # just pass
elif [ -r ../CMakeLists.txt ]; then
  cd .. || exit 2
else
  printf "%s\n" "Neither the current nor the previous directory contains CMakeLists.txt file, exit."
  exit 1
fi

opt="$1"
test_filter="$2"
case "$opt" in
  clean|c)
    [ -d "$bdir" ] && rm -rf "$bdir"
    opt="$2"
    test_filter="$3"
    ;;
esac

cmake -S . -B "$bdir" -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"

# NOTE: not using 'make -C' is my personal preference for such small projects
# 'make -C build' option makes output more noisy - less readable
if [ -n "$NOT_USE_MAKE_C_DIR" ]; then
  cd "$bdir" || exit 3
  make all
  cd ..      || exit 4
else
  make -C "$bdir" all
fi

run_tests="./$bdir/tests/units/algorithms_units"

[ -n "$opt" ] && echo # extra empty line
case "$opt" in
  tests|tests/|test|t)
    if [ -x "$run_tests" ]; then
      if [ -n "$test_filter" ]; then
        "$run_tests" --gtest_filter="$test_filter"
      else
        "$run_tests"
      fi
    else
      printf "%s\n^ %s\n" "$run_tests" \
        "File not found or not executable, exit."
    fi
    ;;
esac
