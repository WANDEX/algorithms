#!/bin/sh
## simple lcov code coverage markdown updater (badge generator)
## https://img.shields.io/badge/coverage-9000%25-brightgreen

set -e

readme=$(realpath "./README.md")

_lcovp=$(lcov --summary ./.coverage/lcov/lcov.info \
-rc lcov_branch_coverage=1 -rc lcov_function_coverage=1 \
| tail -n3 | awk '{print $2}' | cut -d. -f1)

L=$(echo "$_lcovp" | awk 'NR==1') # lines
F=$(echo "$_lcovp" | awk 'NR==2') # funcs
B=$(echo "$_lcovp" | awk 'NR==3') # branches

create_badge() {
  box_txt="$1"
  covp="$2"
  prefix_txt="$3"
  color="lightgrey"
  if   [ "$covp" -ge 95 ]; then
    color="brightgreen"
  elif [ "$covp" -ge 90 ]; then
    color="green"
  elif [ "$covp" -ge 75 ]; then
    color="yellowgreen"
  elif [ "$covp" -ge 60 ]; then
    color="yellow"
  elif [ "$covp" -ge 40 ]; then
    color="orange"
  else
    color="red"
  fi
  url="https://img.shields.io/badge/${prefix_txt}-${covp}%25-${color}"
  ## i.e ![functions](url)
  echo "${box_txt}($url)"
}

L_bdg_txt="![lines    ]"
F_bdg_txt="![functions]"
B_bdg_txt="![branches ]"

_L_bdg_ln=$(grep -Fn "$L_bdg_txt" "$readme")
_F_bdg_ln=$(grep -Fn "$F_bdg_txt" "$readme")
_B_bdg_ln=$(grep -Fn "$B_bdg_txt" "$readme")

L_bdg_lnum=$(echo "$_L_bdg_ln" | cut -d: -f1)
F_bdg_lnum=$(echo "$_F_bdg_ln" | cut -d: -f1)
B_bdg_lnum=$(echo "$_B_bdg_ln" | cut -d: -f1)

L_bdg_old=$(echo "$_L_bdg_ln" | cut -d: -f2-)
F_bdg_old=$(echo "$_F_bdg_ln" | cut -d: -f2-)
B_bdg_old=$(echo "$_B_bdg_ln" | cut -d: -f2-)

L_bdg_new=$(create_badge "$L_bdg_txt" "$L" L)
F_bdg_new=$(create_badge "$F_bdg_txt" "$F" F)
B_bdg_new=$(create_badge "$B_bdg_txt" "$B" B)

i=0
sed_replace_line() {
  ## sed replace whole line (to not interpret as regex)
  ## 1 - line num to replace, 2 - line replacement
  sed -i "${1}s,^.*$,${2}," "$readme"
  i=$((i+1))
}

[ "$L_bdg_old" = "$L_bdg_new" ] || sed_replace_line "$L_bdg_lnum" "$L_bdg_new"
[ "$F_bdg_old" = "$F_bdg_new" ] || sed_replace_line "$F_bdg_lnum" "$F_bdg_new"
[ "$B_bdg_old" = "$B_bdg_new" ] || sed_replace_line "$B_bdg_lnum" "$B_bdg_new"

if [ "$i" = "0" ]; then
  echo "${CYN}Code coverage is equal to the old => no need in update of markdown file.${END}"
  exit 0
fi

echo "${YEL}Code coverage has been updated.${END}"
echo "${YEL}Make commit with README.md changes about code coverage before pushing!${END}"
echo "[README.md] coverage: L:${L}% F:${F}% B:${B}%"
exit 5

