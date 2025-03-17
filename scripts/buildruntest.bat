REM Build script for the test application.
@ECHO OFF
SetLocal EnableDelayedExpansion

cd test

cmake -B build -G "Visual Studio 17 2022
cmake --build build 

IF EXIST build\Debug\testProject.exe (
    build\Debug\testProject.exe
) ELSE (
    ECHO "testProject.exe not found."
)