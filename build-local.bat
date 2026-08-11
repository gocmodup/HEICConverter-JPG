@echo off
REM Build script for Windows
REM Make sure vcpkg is installed and VCPKG_ROOT is set

if not defined VCPKG_ROOT (
    echo Error: VCPKG_ROOT environment variable not set
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows

REM Build
cmake --build . --config Release

cd ..
echo Build complete!