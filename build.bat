@echo off
setlocal

set DIRPATH=Build

if not exist %DIRPATH% (
    mkdir %DIRPATH%
)

cmake -G "MinGW Makefiles" -S . -B %DIRPATH%

mingw32-make -C %DIRPATH%

endlocal
