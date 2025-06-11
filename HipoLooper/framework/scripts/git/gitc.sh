#!/bin/bash

# Check and set COLOR_START and COLOR_END if not already defined
if [ -z "${COLOR_START+x}" ]; then
    unset COLOR_START
    export COLOR_START='\033[35m'
fi

if [ -z "${COLOR_END+x}" ]; then
    unset COLOR_END
    export COLOR_END='\033[0m'
fi

# Define your constant prefix
HipoLooperCodeVersion=14
GIT_COMMIT_PREFIX="[${HipoLooperCodeVersion}_HipoLooper_v${HipoLooperCodeVersion}] "
# GIT_COMMIT_PREFIX="[MyProject] "

echo -e ""
echo -e "${COLOR_START}=======================================================================${COLOR_END}"
echo -e "${COLOR_START}= Updating environment                                                =${COLOR_END}"
echo -e "${COLOR_START}=======================================================================${COLOR_END}"
echo -e ""

echo -e "${COLOR_START}Committing with prefix: ${GIT_COMMIT_PREFIX}${COLOR_END}"
echo -e ""

# Commit with the prefix and user input
git add .
git commit -m "${GIT_COMMIT_PREFIX}$*"
git push
