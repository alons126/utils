#!/bin/bash

# Check and set COLOR_START and COLOR_END if not already defined
if (! $?COLOR_START) then
    unset COLOR_START
    setenv COLOR_START '\033[35m'
endif

if (! $?COLOR_END) then
    unset COLOR_END
    setenv COLOR_END '\033[0m'
endif
