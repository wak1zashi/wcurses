#!/bin/bash

BUILD_DIR_DEBUG="build_gcc_debug"
BUILD_DIR_RELEASE="build_gcc_release"

mkdir -p "$BUILD_DIR_DEBUG"
mkdir -p "$BUILD_DIR_RELEASE"

cd "$BUILD_DIR_DEBUG"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..

cd "$BUILD_DIR_RELEASE"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
cd ..

echo "Done."