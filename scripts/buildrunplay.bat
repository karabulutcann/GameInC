REM Build script for the test application.
@ECHO OFF
SetLocal EnableDelayedExpansion

cd playground

cmake -B build -G "Visual Studio 17 2022
cmake --build build 

IF EXIST build\Debug\playground.exe (
    build\Debug\playground.exe
) ELSE (
    ECHO "playground.exe not found."
)