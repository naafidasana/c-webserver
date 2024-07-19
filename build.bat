@echo off
setlocal

set "SOURCE_DIR=src"
set "BUILD_DIR=build"
set "CMAKE_LISTS_PATH=CMakeLists.txt"

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

cd "%BUILD_DIR%"

cmake .. -G "Ninja"  :: Use "Ninja"

cmake --build . --config Release

endlocal
