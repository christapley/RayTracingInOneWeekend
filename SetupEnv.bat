@echo off


rem call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

set CMAKE=F:\Dev\cmake-3.16.1-win64-x64\bin\cmake
set GENERATOR=Visual Studio 16 2019
rmdir /S /Q build

%CMAKE% -G "%GENERATOR%" -B build -A x64

