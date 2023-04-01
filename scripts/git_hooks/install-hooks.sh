#!/bin/sh
## install all custom git hooks

set -e

git_dir=$(realpath "$(git rev-parse --git-dir)")

# full path to the dir of this file
dpath=$(dirname "$(realpath "$0")")

echo "Installing git hooks..."

ln -s "${dpath}/pre-push" "${git_dir}/hooks/pre-push"

echo "Done!"
