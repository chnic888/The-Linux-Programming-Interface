#!/bin/sh

BUILD_DIR='./build'
START=3
END=24

if [ $# -eq 1 ]; then
    START="$1"
fi

if [ $# -eq 2 ]; then
    END="$2"
fi

if [ -d "$BUILD_DIR" ]; then
  printf "build folder cleanup\n"
  rm -rf "$BUILD_DIR"
fi

mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || exit
cmake .. -DSTART="$START" -DEND="$END"
make
