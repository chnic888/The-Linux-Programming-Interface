#!/bin/sh

BUILD_DIR='./build'

if [ -d "$BUILD_DIR" ]; then
  printf "build folder cleanup\n"
  rm -rf "$BUILD_DIR"
fi

mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || exit
cmake ..
make
