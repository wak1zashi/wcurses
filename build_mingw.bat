@echo off

set BUILD_DIR_DEBUG=build_mingw_debug
set BUILD_DIR_RELEASE=build_mingw_release

if not exist %BUILD_DIR_DEBUG% mkdir %BUILD_DIR_DEBUG%
if not exist %BUILD_DIR_RELEASE% mkdir %BUILD_DIR_RELEASE%

cd %BUILD_DIR_DEBUG%
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
mingw32-make
cd ..

cd %BUILD_DIR_RELEASE%
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
cd ..

echo Done.
pause