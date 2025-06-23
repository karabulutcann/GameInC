@REM REM Build script for the test application.
@REM @ECHO OFF
@REM SetLocal EnableDelayedExpansion

@REM REM Get a list of all the .cpp files.
@REM SET cFilenames=
@REM FOR /R %%f in (*.c) do (
@REM     SET cFilenames=!cFilenames!"%%f" 
@REM )

@REM SET assembly=test
@REM SET compilerFlags=
@REM REM Define what to include.
@REM SET includeFlags=-Isrc -Iinclude 
@REM REM Define what to link.
@REM SET linkerFlags=-luser32 -lkernel32 -lgdi32 -lshell32 -Llib/GLFW -lglfw3_mt -lmsvcrtd -llibcmtd
@REM REM Define some extra stuff.
@REM SET defines=-D_DEBUG -Xlinker /NODEFAULTLIB:libcmt

@REM REM Build the project.
@REM ECHO "Building %assembly%%..."
@REM clang %cFilenames% %compilerFlags% -o bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%

@REM @REM xcopy /s /y shaders\* bin\shaders\