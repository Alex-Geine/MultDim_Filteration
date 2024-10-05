#!/bin/bash
# our comment is here
mkdir libs/Picture/build
cd libs/Picture/build
cmake ..
make
cd ../../../

mkdir libs/Signal/build
cd libs/Signal/build
cmake ..
make
cd ../../../

mkdir build/
cd build
cmake ..
make
