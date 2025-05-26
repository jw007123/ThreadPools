@echo off

mkdir External
git clone https://gitlab.com/libeigen/eigen.git External/eigen
git clone https://github.com/wolfpld/tracy.git External/tracy

mkdir Build
cd Build
cmake ..
make