@echo off
setlocal

set GS_VER=gs10051w64.exe
set GS_URL=https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/latest/download/%GS_VER%

echo Downloading Ghostscript...
curl -L -o %GS_VER% %GS_URL%

echo Installing...
%GS_VER% /VERYSILENT /NORESTART
del %GS_VER%

echo Done.
endlocal
pause
