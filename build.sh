#!/bin/bash
# our comment is here
cd libs/Picture/build
cmake ..
make
cd ../../../

cd libs/Signal/build
cmake ..
make
cd ../../../

cd build
cmake ..
make
