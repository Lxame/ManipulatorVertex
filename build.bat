@echo off
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -T v142 ..
cmake --build . --config Debug
cmake --build . --config Release