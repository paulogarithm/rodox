#!/bin/bash

HERE=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd);

if [ $# -lt 2 ] || [ ! -f $HERE/Makefile ]; then
    exit 1;
fi;

ACTION=$1;

for ((i=2; i<=$#; ++i)); do
    ARG="${@:i:1}";
    ARG=$(basename ${ARG%/});
    MOD=$ARG make -C $HERE $ACTION;
done;

