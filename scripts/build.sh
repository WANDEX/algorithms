#!/bin/sh
## build & optionally run tests.
## NOTE: manually export following environment variables
## to build/test under multiple compilers before submitting code!
#
# export CC=gcc   CXX=g++
# export CC=clang CXX=clang++
#
## build:
# ./scripts/build.sh
## build & run tests:
# ./scripts/build.sh ctest
## make clean build & run tests:
# ./scripts/build.sh clean ctest
## build & run filtered tests:
# ./scripts/build.sh ctest .*regex.*
## or
# ./scripts/build.sh gtest *wildcard*

bt="Debug"
compiler="${CC:-_}"
bdir="build/dev-$bt-($compiler)"

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
fresh=""
clean_first=""
case "$opt" in
  clean|c)
    opt="$2"
    test_filter="$3"
    fresh="--fresh"
    clean_first="--clean-first"
    ;;
esac

und='=========================='
sep="${und}${und}${und}"
vsep() { printf "\n%s[%s]\n%s\n\n" "${2}" "${1}" "${sep}${END}" ;}

vsep "CONFIGURE" "${BLU}"
cmake -S . -B "$bdir" -G Ninja -D CMAKE_BUILD_TYPE=${bt} -Wdev -Werror=dev ${fresh}

vsep "BUILD" "${CYN}"
cmake --build "$bdir" --config ${bt} ${clean_first}

gtest_binary="./$bdir/tests/units/algorithms_units"
if [ ! -x "$gtest_binary" ]; then
  printf "%s\n^ %s\n" "$gtest_binary" \
    "File not found or not executable, exit."
  exit 5
fi

[ -n "$opt" ] && vsep "TESTS" "${RED}"
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
