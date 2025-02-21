REM Build script for the test application.
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .cpp files.
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames!"%%f" 
)

SET assembly=test
SET compilerFlags=
REM Define what to include.
SET includeFlags=-Isrc -Iinclude 
REM Define what to link.
SET linkerFlags=-luser32 -lkernel32 -lgdi32 -lshell32 -Llib/GLFW -lglfw3_mt -lmsvcrtd -llibcmtd
REM Define some extra stuff.
SET defines=-D_DEBUG -Xlinker /NODEFAULTLIB:libcmt

REM Build the project.
ECHO "Building %assembly%%..."
clang %cFilenames% %compilerFlags% -o bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%

@REM xcopy /s /y shaders\* bin\shaders\