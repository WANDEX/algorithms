#!/bin/sh
## gcov wrapper - support any gcov argument as script options

set -e

at_path() { hash "$1" >/dev/null 2>&1 ;} # if $1 is found at $PATH -> return 0

if ! at_path gcov; then
  echo "gcov not found at PATH! Exit."
  exit 1
fi

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
[ -d "$gcov_dir" ] && rm -rf "$gcov_dir"
mkdir -p "$gcov_dir"
cd "$gcov_dir" || exit 12

# shellcheck disable=SC2068 # Intentional - to re-split trailing arguments.
# NOTE: pass -b as argument of the script to see branch info
if at_path fd; then
  fd . "$bdir" -tf -e gcda -x gcov -s "$incl_dir" "$flags" $@

elif at_path find && at_path xargs; then
  find "$bdir" -name '*.gcda' -print0 | xargs -0 gcov -s "$incl_dir" "$flags" $@
fi

