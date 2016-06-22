#!/bin/bash

if [ $# -lt 1 ]; then
    printf "Usage:\n$0 file.txt\n"
    exit 1
fi

IN=$1
OUT=${1%%.*}
ASM=$OUT.s

./etapa6 $IN $ASM

ERROR=$?
if [ $ERROR -ne 0 ]; then
    exit $ERROR
fi

gcc $ASM -o $OUT
