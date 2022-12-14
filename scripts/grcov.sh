#!/bin/sh
## grcov wrapper - support any grcov arguments as script options
## prefer to use lcov.sh!
##
## NOTE DO NOT USE: with --branch option - configured improperly for this!
## does not override/use:
## geninfo_external=0 geninfo_no_exception_branch=1
##
## We do not want to test standard library, 3rd party libraries (gtest), and care
## about almost impossible exception branches like std::bad_alloc etc.
## All those uncovered branches come from this!
##
## It also does not show exactly which functions not covered... (dunno why)
## So for me - this tool is practically useless.

set -e

at_path() { hash "$1" >/dev/null 2>&1 ;} # if $1 is found at $PATH -> return 0

if ! at_path grcov; then
  echo "grcov not found at PATH! Exit."
  exit 1
fi

bdir="./build/this-was-unexpected/"
case "$CC" in
  *clang) bdir=$(realpath "./build/dev-Debug-clang/") ;;
  *gcc)   bdir=$(realpath "./build/dev-Debug-gcc/")   ;;
  *) echo "Compiler is not supported by grcov, exit." && exit 10 ;;
esac

if [ ! -d "$bdir" ]; then
  printf "%s\n%s\n" "hardcoded build dir does not exist:" "$bdir"
  exit 11
fi

incl_dir=$(realpath "./include")
grcov_dir=$(realpath "./.coverage/grcov")
[ -d "$grcov_dir" ] && rm -rf "$grcov_dir"

# shellcheck disable=SC2068 # Intentional - to re-split trailing arguments.
grcov "$bdir" -s "$incl_dir" -t html -o "$grcov_dir" \
--excl-line     LCOV_EXCL_LINE \
--excl-start    LCOV_EXCL_START \
--excl-stop     LCOV_EXCL_STOP \
--excl-br-line  LCOV_EXCL_BR_LINE \
--excl-br-start LCOV_EXCL_BR_START \
--excl-br-stop  LCOV_EXCL_BR_STOP $@

