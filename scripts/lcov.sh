#!/bin/sh
## lcov wrapper - support any lcov arguments as script options

set -e

at_path() { hash "$1" >/dev/null 2>&1 ;} # if $1 is found at $PATH -> return 0

if ! at_path lcov; then
  echo "lcov not found at PATH! Exit."
  exit 9
fi

bdir=$(./scripts/wndx_cmake_build.sh --get_build_dir)
if [ ! -d "$bdir" ]; then
  printf "%s\n%s\n" "build dir does not exist:" "$bdir"
  exit 11
fi
case "$CC" in
  *gcc|*clang) # bypass
  ;;
  *)
    echo "only gcc & clang compiler supported by lcov! EXIT."
    exit 10
  ;;
esac

ccov_dir=$(realpath "./.coverage")

lcov_dir="${ccov_dir}/lcov"
lcov_info="${lcov_dir}/lcov.info"
[ -d "$lcov_dir" ] &&   rm -rf "$lcov_dir"
[ -d "$lcov_dir" ] || mkdir -p "$lcov_dir"

lcov_ignore_errors="unused,inconsistent,inconsistent,mismatch,mismatch"

# shellcheck disable=SC2068 # Intentional - to re-split trailing arguments.
lcov -q -c --directory "$bdir" --output-file "$lcov_info" --demangle-cpp \
--exclude /usr/ --exclude /tests/ \
--exclude third_patry --exclude 3rdparty --exclude _deps \
--ignore-errors "$lcov_ignore_errors" \
--rc branch_coverage=1 --rc function_coverage=1 \
--rc geninfo_external=1 --rc no_exception_branch=1 \
--rc geninfo_unexecuted_blocks=1 --rc geninfo_auto_base=1 $@

genhtml "$lcov_info" --output-directory "$lcov_dir" --demangle-cpp \
--branch-coverage --legend --dark-mode

## path to the index.html to easily copy-paste into the browser (if needed)
printf "\n%s\n" "${lcov_dir}/index.html"
