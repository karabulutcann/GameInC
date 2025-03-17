REM Build script for the test application.
@ECHO OFF
SetLocal EnableDelayedExpansion


cmake -B build -G Ninja
cmake --build build 

IF EXIST build\Debug\TestApp.exe (
    build\Debug\TestApp.exe
) ELSE (
    ECHO "TestApp.exe not found."
)

IF EXIST build\compile_commands.json (
    COPY build\compile_commands.json .
) ELSE (
    ECHO "compile_commands.json not found."
)
