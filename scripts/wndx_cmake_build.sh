#!/bin/sh
## build & optionally run tests. AUTHOR: WANDEX.
## Script is meant only for the project developer and as the reference.
## No guarantees that it will work on your OS/platform!
## Use standard cmake commands for building the project.
## Or adapt script steps yourself for your personal preference.
##
## NOTE: manually export following environment variables
## or pass -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++
## to build/test under multiple compilers before submitting code!
#
# export CC=gcc   CXX=g++
# export CC=clang CXX=clang++
# export CC=cl    CXX=cl
#
## example MSVC compilation on win10 platform in git-bash shell:
# export PATH0="$PATH" # backup unmodified PATH
# export PATH="$PATH:/c/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.36.32532/bin/Hostx64/x64"
# export PATH="$PATH:/c/wndx/src/top/DrMemory-Windows-2.6.0/bin64"
# export PATH="$PATH:/c/wndx/src/top/DrMemory-Windows-2.6.0/bin"
# rm -rf ./build/msvc/ && mkdir -p ./build/msvc/
# cmake -S . -B ./build/msvc/ -G "Visual Studio 17 2022" -DWNDX_ALGO_BUILD_TESTS=ON -DWNDX_ALGO_MEMCHECK_ENABLE=ON
# cmake --build ./build/msvc/
# ctest --test-dir ./build/msvc/tests/units/
# cmake --build ./build/msvc/ --target memcheck
#
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
##
# CC=cl CXX=cl VERBOSE=1 DEPLOY=1 BUILD_TYPE=Debug BUILD_TESTS="WNDX_ALGO_BUILD_TESTS" GENERATOR="Visual Studio 17 2022" ./scripts/build.sh c ct

set -e

bname=$(basename "$0")
USAGE="\
Usage: $bname [OPTION...]
OPTIONS
    --get-build-dir           print build dir
    --get-project-name        print project name parsed from CMakeLists.txt
    --get-project-name-upper  print project name in uppercase
    -h, --help                print help
POSITIONAL
    c , clean  , --clean
    cc, cleaner, --cleaner
    ctest, ct, ctp, ctr       (optional) arg - filter regex
    gtest, gt                 (optional) arg - filter wildcard (:,*,?,-)
                              ':'-separated patterns '*', '?' ('-' negative)
EXAMPLES
$bname c ct .*
$bname c gt *
"

OLDIFS="$IFS"

if [ ! -r ./CMakeLists.txt ]; then
  printf "%s\n" "Current directory does not contain CMakeLists.txt, exit."
  exit 1
fi

at_path() { hash "$1" >/dev/null 2>&1 ;} # if $1 is found at $PATH -> return 0

get_prj_name() {
  if at_path rg; then
    _prj_name=$(rg -Ue '\bproject\([\s]*[\n]*\b(\w+)\b' --trim -or '$1' ./CMakeLists.txt)
  else
    _prj_name=$(basename "$(git rev-parse --show-toplevel)")
  fi
  printf "%s" "$_prj_name"
}

notify() {
  EC="$?" ## save error/exit code of the previously executed command
  at_path notify-send || return 0
  tag="string:x-dunst-stack-tag"
  sbg="string:bgcolor:"
  sfg="string:fgcolor:"
  arg="$2"
  ##
  case "$1" in
  error|ERROR)     urg="critical" ;;
  warning|WARNING) urg="normal"   ;;
  *)
    arg="$1"
    urg="low"
    sbg="string:bgcolor:#11FF44"
    sfg="string:fgcolor:#000000"
  ;;
  esac
  notify-send -u "$urg" -h "$tag:$PRJ_NAME" -h "$tag:hi" -h "$sbg" -h "$sfg" "[$PRJ_NAME]" "$arg"
}

# NOTE: on win10 in MINGW64:/git-bash shell -- trailing args via -- '$*' - works; '$@' - NOT works;
# ^ apparently in this environment - this function is still not always finds tests...
# shellcheck disable=SC2048,SC2068,SC2086 # intentional - to re-split trailing arguments.
run_ctest() { cmake -E time ctest --build-config "$BUILD_TYPE" --output-on-failure --test-dir "$_bdir/$TESTS_DIR" $* ;}

SEP="=============================================================================="
vsep() { printf "\n%b%.78s%b\n\n" "${2}" "[${1}]${SEP}" "${END}" ;}

pre_configure() {
  ## cmake verbosity level
  _verbose=""
  _cmake_log_level=""
  if [ "$VERBOSE" = 1 ]; then
    _verbose="--verbose"
    _cmake_log_level="--log-level=DEBUG"
  fi
  ## cmake clean options
  _fresh=""
  _clean_first=""
  if [ "$CLEAN" = 1 ]; then
    _fresh="--fresh"
    _clean_first="--clean-first"
  fi
  ## cleaner is a heavy artillery - for edge cases!
  if [ "$CLEANER" = 1 ]; then
    [ -d "$BUILD_DIR" ] &&   rm -rf "$BUILD_DIR"
    [ -d "$BUILD_DIR" ] || mkdir -p "$BUILD_DIR"
  fi
  ## source and build dirs provided as the relative paths
  _sdir="."
  _bdir="$BUILD_DIR"
  if [ "$REL" = 1 ]; then
    cd "$BUILD_DIR" || exit 11
    pwd # cd - cmd visual feedback about internal behavior
    _sdir="../../.."
    _bdir="."
  fi
}

trailing_args() {
  targc="${#}" # arguments count
  targv="${*}" # arguments concatenated string
  [ -z "$targv" ] && return
  argn=0
  set -o noglob # disable (wildcard expansion)
  IFS=" " # split arguments by space
  for arg in $targv; do
    argn=$((argn+1))
    true && printf "\ntargc=%d targv='%s' argv[%d]='%s'\n" "$targc" "$targv" "$argn" "$arg" # debug
    if [ -n "$arg_type" ]; then # => filter pattern: ctest regex or gtest wildcard
      TESTS_FILTER="$arg"
      arg_type="" # set tmp var type - back to empty
      continue
    fi
    case "$arg" in
    ctest|ctp|ctr|ct|gtest|gt)
      arg_type="$arg" # tmp var type
      RUN_TESTS=1
      RUN_TESTS_TYPE="$arg_type"
      ## test filter pattern will be set ON NEXT LOOP ITERATION! => next $arg is filter (optional).
      case "$arg_type" in gtest|gt) TESTS_FILTER='*' ;; esac # gtest - default wildcard
      continue
    ;;
    cleaner|cc)
      CLEANER=1
      continue
    ;;
    clean|c)
      CLEAN=1
      continue
    ;;
    *)
      targv=$(echo "$targv" | cut -d ' ' -f "${argn}-")
      printf "unparsed trailing args[%d]=[%s]\nunsupported arg: [%s]\n" "$targc" "$targv" "$arg"
      exit 6
    ;;
    esac
  done
  set +o noglob # restore - enable (wildcard expansion)
  IFS="$OLDIFS" # restore
  if true; then # debug
    echo "CLEAN=$CLEAN CLEANER=$CLEANER RUN_TESTS_TYPE=$RUN_TESTS_TYPE TESTS_FILTER=$TESTS_FILTER"
    exit 33
  fi
}

get_opt() {
  ## Parse and read OPTIONS command-line options
  SHORT=h
  LONG=clean,cleaner,get-build-dir,get-project-name,get-project-name-upper,help
  OPTIONS=$(getopt --options $SHORT --long $LONG --name "$0" -- "$@")
  ## PLACE FOR OPTION DEFAULTS BEG
  TESTS_DIR="${TESTS_DIR:-tests/units}"
  BUILD_TYPE="${BUILD_TYPE:-Debug}"
  GENERATOR="${GENERATOR:-Ninja}" # "MSYS Makefiles", "Unix Makefiles", "Ninja Multi-Config", Ninja
  VERBOSE="${VERBOSE:-0}"
  DEPLOY="${DEPLOY:-0}"
  REL="${REL:-0}"
  ##
  CLEAN=0
  CLEANER=0
  ##
  RUN_TESTS=0
  RUN_TESTS_TYPE=''
  TESTS_FILTER='.*'
  ##
  _compiler="${CC:-_}"
  cmbn=$(basename "$_compiler") # get compiler basename in case declared via full path
  gen_dir_name=$(echo "$GENERATOR" | sed "s/[ |-]/_/g" | tr "[:upper:]" "[:lower:]")
  arch=$(uname -m)
  BUILD_DIR="build/${gen_dir_name}_${arch}/dev_${cmbn}_${BUILD_TYPE}" # XXX: old
  # TODO: do not append BUILD_TYPE if Multi-Config Generator is used
  # BUILD_DIR="build/${gen_dir_name}_${arch}/${cmbn}_${BUILD_TYPE}"
  [ -d "$BUILD_DIR" ] || mkdir -p "$BUILD_DIR"
  ##
  PRJ_NAME=$(get_prj_name)
  PRJ_NAME_UPPER=$(echo "$PRJ_NAME" | tr "[:lower:]" "[:upper:]")
  PRJ_BUILD_TESTS="${BUILD_TESTS:-${PRJ_NAME_UPPER}_BUILD_TESTS}"
  PRJ_MEMCHECK_EN="${MEMCHECK:-${PRJ_NAME_UPPER}_MEMCHECK_ENABLE}"
  COPTS=\
"-D CMAKE_BUILD_TYPE=$BUILD_TYPE
 -D $PRJ_BUILD_TESTS=$RUN_TESTS
 -D $PRJ_MEMCHECK_EN=ON
 $COPTS
" ## <- default cmake configure options
  BOPTS=\
"$BOPTS
" ## <- default cmake build options
  ## PLACE FOR OPTION DEFAULTS END
  eval set -- "$OPTIONS"
  while true; do
    case "$1" in
    --clean)
      CLEAN=1
    ;;
    --cleaner)
      CLEANER=1
    ;;
    --get-build-dir)
      printf "%s" "$BUILD_DIR"
      exit 0
    ;;
    --get-project-name)
      printf "%s" "$PRJ_NAME"
      exit 0
    ;;
    --get-project-name-upper)
      printf "%s" "$PRJ_NAME_UPPER"
      exit 0
    ;;
    -h|--help)
      echo "$USAGE"
      exit 0
    ;;
    --)
      shift
      trailing_args "$*"
      break
    ;;
    esac
    shift
  done
  pre_configure
}

get_opt "$@"


vsep "CONFIGURE" "${BLU}"
# shellcheck disable=SC2086 # Intentional - re-split OPTIONS CONFIGURE
cmake -E time \
cmake -S "$_sdir" -B "$_bdir" -G "$GENERATOR" -Wdev -Werror=dev \
${_fresh} ${_cmake_log_level} ${COPTS} \
|| { notify ERROR "CMAKE CONFIGURE ERROR" ; exit "$EC" ;}

vsep "BUILD" "${CYN}"
# shellcheck disable=SC2086 # Intentional - re-split OPTIONS BUILD
cmake -E time \
cmake --build "$_bdir" --config "${BUILD_TYPE}" ${_clean_first} ${_verbose} ${BOPTS} \
|| { notify ERROR "CMAKE BUILD ERROR" ; exit "$EC" ;}

if [ "$RUN_TESTS" = 1 ]; then
  vsep "TESTS" "${RED}"
  case "$RUN_TESTS_TYPE" in
  ct|ctest) # optionally filter by regex
    run_ctest -R "$TESTS_FILTER"
  ;;
  ctp) # shortcut for terse progress output
    run_ctest --progress -R "$TESTS_FILTER"
  ;;
  ctr) # suppress stdout/stderr, EC -> always success (to not immediately exit because of 'set -e')
    run_ctest -R "$TESTS_FILTER" >/dev/null 2>&1 || true
    run_ctest --rerun-failed # rerun previously failed - (filtered) results, with only failed tests.
  ;;
  gt|gtest)
    gtest_binary="./$_bdir/bin/tests_units"
    if [ ! -x "$gtest_binary" ]; then
      printf "%s\n^ %s\n" "$gtest_binary" "File not found or not executable, exit."
      exit 5
    fi
    "$gtest_binary" --gtests_filter="$TESTS_FILTER"
  ;;
  esac
fi

if [ "$DEPLOY" = 1 ]; then
  vsep "DEPLOY" "${MAG}"
  cmake --install "$_bdir" --config "${BUILD_TYPE}" --prefix "$_bdir/deploy"
fi

vsep   "COMPLETED" "${GRN}"
notify "COMPLETED"
