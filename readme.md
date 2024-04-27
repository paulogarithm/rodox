```sh
export LUA_CPATH="`pwd`/lib/?.so;;"
export LUA_PATH="`pwd`/lib/?.lua;;"
export LD_LIBRARY_PATH=`pwd`/build:$LD_LIBRARY_PATH
```