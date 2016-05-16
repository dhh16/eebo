#!/bin/sh

rm -rf build
mkdir build || exit 1
cd build || exit 1
CXX="g++-5" CXXFLAGS="-fopenmp -fdiagnostics-color=always" LDFLAGS="-fopenmp" meson --buildtype=debugoptimized .. || exit 1
ninja || exit 1
