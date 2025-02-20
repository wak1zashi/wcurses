@echo off

set BUILD_DIR=build_vs_17

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

cd %BUILD_DIR%
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Debug
cmake --build . --config Release
cd ..

echo Done.
pause