@echo off
rem Creates a solution for Visual Studio 2015 v140 x64.
set toolset=v140

rem Creates a solution for Visual Studio 2015 v140_xp x86.
rem set toolset=v140_xp

if "%toolset%" == "v140" (
  echo Generating a Visual Studio 2015 v140 x64 solution.
  set cmake_generator="Visual Studio 14 2015 Win64"
  set cmake_toolset="v140"
) else if "%toolset%"=="v140_xp" (
  echo Generating a Visual Studio 2015 v140_xp x86 solution.
  set cmake_generator="Visual Studio 14 2015"
  set cmake_toolset="v140_xp"
) else (
  echo Please modify this file ^(%~n0.bat^) and select a toolset.
  pause
  exit 1
)

rem Enter the script directory.
pushd %~dp0

rem Enter the build directory.
if not exist build (
  mkdir build
)
pushd build

rem Generate the solution.
cmake -G %cmake_generator% -T %cmake_toolset% -DCMAKE_INSTALL_PREFIX:PATH=.. ..
if %errorlevel% == 0 (
  start %~n0.sln
) else (
  pause
)

rem Leave the build directory.
popd
