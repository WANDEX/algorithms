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

gtest_binary="./$bdir/tests/units/algorithms_units"
if [ ! -x "$gtest_binary" ]; then
  printf "%s\n^ %s\n" "$gtest_binary" \
    "File not found or not executable, exit."
  exit 5
fi

[ -n "$opt" ] && echo # extra empty line
case "$opt" in
  ctest|ct)
    if [ -n "$test_filter" ]; then # regex
      ctest --test-dir "$bdir" -R "$test_filter"
    else
      ctest --test-dir "$bdir"
    fi
    ;;
  gtest|gt)
    if [ -n "$test_filter" ]; then
      # wildcard ':'-separated patterns '*', '?' ('-' negative)
      "$gtest_binary" --gtest_filter="$test_filter"
    else
      "$gtest_binary"
    fi
    ;;
esac
