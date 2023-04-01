#!/bin/sh
## install all custom git hooks

set -e

git_dir=$(realpath "$(git rev-parse --git-dir)")

# full path to the dir of this file
dpath=$(dirname "$(realpath "$0")")
cd "$dpath" || exit 2

echo "Installing git hooks..."

# this command create symlink to our script and makes it a pre-push hook
ln -s "${dpath}/hk_coverage" "${git_dir}/hooks/pre-push-hk_coverage"

echo "Done!"
