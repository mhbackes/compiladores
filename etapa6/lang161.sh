#!/bin/bash

if [ $# -lt 1 ]; then
    printf "Usage:\n$0 file.txt\n"
    exit 1
fi

IN=$1
OUT=${1%%.*}
ASM=$OUT.s

echo "etapa6 $IN $ASM"
./etapa6 $IN $ASM

ERROR=$?
if [ $ERROR -ne 0 ]; then
    exit $ERROR
fi

echo "gcc $ASM -o $OUT -g"
gcc $ASM -o $OUT -g
