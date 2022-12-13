#!/bin/sh

set -e

# gcov flags to enable colors via ascii escape sequences
TERM_COLORS=1
if [ "$TERM_COLORS" -eq 1 ]; then
  flags="-qHkr"
else
  flags="-Hr"
fi

bdir="./build/this-was-unexpected/"
case "$CC" in
  *gcc) bdir=$(realpath "./build/dev-Debug-gcc/") ;;
  *) echo "Only GCC compiler is supported by gcov! Exit." && exit 10 ;;
esac

if [ ! -d "$bdir" ]; then
  printf "%s\n%s\n" "hardcoded build dir does not exist:" "$bdir"
  exit 11
fi

incl_dir=$(realpath "./include")
ccov_dir=$(realpath "./.coverage")
gcov_dir="${ccov_dir}/gcov"
lcov_dir="${ccov_dir}/lcov"
lcov_info="${lcov_dir}/lcov.info"

[ -d "$gcov_dir" ] && rm -rf "$gcov_dir"
[ -d "$lcov_dir" ] && rm -rf "$lcov_dir"
mkdir -p "$gcov_dir"
mkdir -p "$lcov_dir"
cd "$gcov_dir" || exit 12

# covp=$(find "$bdir" -name '*.gcda' -print0 | xargs -0 gcov | tail -n 1 | awk '{sub(/executed:/,""); printf "%d", $2}')

# shellcheck disable=SC2068 # Intentional - to re-split trailing arguments.
# NOTE: pass -b to see branch info
fd . "$bdir" -tf -e gcda -x gcov -s "$incl_dir" "$flags" $@

# extra empty line
echo ""

lcov --directory "$bdir" --capture --output-file "$lcov_info" -rc lcov_branch_coverage=1
# remove output for external libraries (i.e. gtest)
lcov --remove "$lcov_info" "/usr*" -o "$lcov_info"
genhtml "$lcov_info" --branch-coverage --output-directory "$lcov_dir"

