#!/bin/bash

# Set installation directories
LUA_SUBVER=5.4
LUA_VERSION=$LUA_SUBVER.4
LUA=lua-$LUA_VERSION
LUATAR=$LUA.tar.gz
HERE=`pwd`
LUA_DIR=$HERE/lua
LUALIB_DIR=$LUA_DIR/lib
LUASRC_DIR=$LUA_DIR/src
SHARE_DIR=$LUA_DIR/include

# Download Lua source code
wget https://www.lua.org/ftp/$LUATAR
if [ ! -f "$LUATAR" ]; then
    echo "$LUATAR not found"
    exit 1
fi;
tar -xzvf $LUATAR

# Remove the old folder and the tarball
if [ -d "$LUA_DIR" ]; then
    rm -rf $LUA_DIR
fi;
mv $LUA $LUA_DIR
rm -rf $LUATAR

# Configure and compile Lua
cd $LUA_DIR
make linux MYCFLAGS=-fPIC MYLDFLAGS=-static
make install INSTALL_TOP=$LUA_DIR
echo "Installation complete."

# Building

BIN=cfoo.so
LIB=$HERE/lib
SRC=$HERE/src
export LUA_CPATH="$HERE/lib/?.so;;"

gcc -shared -fPIC -o $LIB/cfoo.so $SRC/cfoo.c -L$LUALIB_DIR -llua -I$SHARE_DIR

