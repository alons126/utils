#!/bin/bash

# source "$(dirname "$0")/../environment/system_colors.sh"
# source ../environment/system_colors.sh

# Define your constant prefix
HipoLooperCodeVersion=14
GIT_COMMIT_PREFIX="[${HipoLooperCodeVersion}_HipoLooper_v${HipoLooperCodeVersion}] "
# GIT_COMMIT_PREFIX="[MyProject] "
# echo -e "${COLOR_START}Committing with prefix: ${GIT_COMMIT_PREFIX}${COLOR_END}"

# Commit with the prefix and user input
git add .
git commit -m "${GIT_COMMIT_PREFIX}$*"
git push
