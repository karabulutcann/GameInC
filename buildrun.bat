REM Build script for the test application.
@ECHO OFF
SetLocal EnableDelayedExpansion


cmake -B build -G "Visual Studio 17 2022"
cmake --build build 

IF EXIST build\Debug\TestApp.exe (
    build\Debug\TestApp.exe
) ELSE (
    ECHO "TestApp.exe not found."
)