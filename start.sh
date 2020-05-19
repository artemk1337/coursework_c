#!/bin/sh
rm -rf build generator lem-in
mkdir build
cd ./build/
pwd
cmake ..
make
cp "project/lem-in" ../
cd ..
make -C generator_src
