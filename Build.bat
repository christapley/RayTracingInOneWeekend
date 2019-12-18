@echo off

set CMAKE_DIR=F:\Dev\cmake-3.16.1-win64-x64\bin
set BUILD_DIR=%cd%\build

%CMAKE_DIR%\cmake --build %BUILD_DIR% --config %1
pushd %BUILD_DIR%\src
%CMAKE_DIR%\ctest 
popd

