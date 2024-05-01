#!/bin/bash

CC=$(which cc);
ARG=$(basename ${1%/};)
MODULEDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd);
SRCDIR=$(dirname $MODULEDIR)
BIN="$ARG.so";
FILESDIR="${MODULEDIR}/${ARG}"
RODOX=$(dirname $SRCDIR)
LIBDIR="$RODOX/lib"
COMMONDIR="${SRCDIR}/common"
COMMONFILES=$(find $COMMONDIR -name "*.c")

if [ ! $1 ] || [ ! -d $FILESDIR ]; then
    exit 1;
fi;

$CC $(echo $FILESDIR/*.c) ${COMMONFILES} \
    -shared -fpic -iquote${COMMONDIR} \
    -o "$LIBDIR/$BIN" 
