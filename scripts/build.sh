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
## make cleaner build (for edge cases) & run tests:
# ./scripts/build.sh cleaner ctest
## build & run filtered tests:
# ./scripts/build.sh ctest .*regex.*
## or
# ./scripts/build.sh gtest *wildcard*

bt="Debug"
compiler="${CC:-_}"
# get compiler basename in case declared via full path
cmbn=$(basename "$compiler")
bdir="build/dev-$bt-$cmbn"

set -e

beg_epoch=$(date +%s) # for the first call of the time_spent_on_step

if [ -r ./CMakeLists.txt ]; then
  _="" # just pass
elif [ -r ../CMakeLists.txt ]; then
  cd .. || exit 2
else
  printf "%s\n" "Neither the current nor the previous directory contains CMakeLists.txt file, exit."
  exit 1
fi

[ -n "$CTEST_OUTPUT_ON_FAILURE" ] || export CTEST_OUTPUT_ON_FAILURE=1

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
  cleaner|cc) # for edge cases like corrupted GCDA files etc.
    opt="$2"
    test_filter="$3"
    [ -d "$bdir" ] && rm -rf "$bdir"
    mkdir -p "$bdir"
    ;;
esac

time_spent_on_step() {
  # HACK: because standard utility/command 'time' does not work.
  end_epoch=$(date +%s)
  diff_secs=$((end_epoch-beg_epoch))
  # small insignificant optimization
  [ "$diff_secs" = 0 ] && beg_epoch="$end_epoch" && return
  spent_time=$(date -d "@${diff_secs}" "+%Mm %Ss")
  beg_epoch=$(date +%s)
  printf "step took:%b %s.%b\n" "${BLD}" "$spent_time" "${END}"
}

und='=========================='
sep="${und}${und}${und}"
vsep() {
  time_spent_on_step
  printf "\n%s[%s]\n%s\n\n" "${2}" "${1}" "${sep}${END}"
}

vsep "CONFIGURE" "${BLU}"
cmake -S . -B "$bdir" -G Ninja -D CMAKE_BUILD_TYPE=${bt} -Wdev -Werror=dev ${fresh}

vsep "BUILD" "${CYN}"
cmake --build "$bdir" --config ${bt} ${clean_first}

gtest_binary="./$bdir/tests/units/wndxlib_units"
if [ ! -x "$gtest_binary" ]; then
  printf "%s\n^ %s\n" "$gtest_binary" \
    "File not found or not executable, exit."
  exit 5
fi

# shellcheck disable=SC2068 # Intentional - to re-split trailing arguments.
run_ctest() { ctest --test-dir "$bdir" -t code_coverage $@ ;} # shortcut

[ -n "$opt" ] && vsep "TESTS" "${RED}"
[ -n "$test_filter" ] || test_filter='.*'
case "$opt" in
  ctest|ct)
    # optionally filter by regex
    run_ctest -R "$test_filter"
    ;;
  ctp)
    # shortcut for terse progress output
    run_ctest --progress -R "$test_filter"
    ;;
  ctr)
    # suppress output, errors, always return success (to not immediately exit because of 'set -e')
    run_ctest -R "$test_filter" >/dev/null 2>&1 || true
    # rerun previously failed (gives live (filtered) results, with only failed tests)
    run_ctest --rerun-failed
    ;;
  gtest|gt)
    # optionally filter by wildcard ':'-separated patterns '*', '?' ('-' negative)
    [ "$test_filter" = '.*' ] && test_filter='*'
    "$gtest_binary" --gtest_filter="$test_filter"
    ;;
esac
[ -n "$opt" ] && vsep "COMPLETED" "${GRN}"

