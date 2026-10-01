@echo off
setlocal enabledelayedexpansion

rem ===========================================================================
rem  stage_mpv.bat
rem
rem  Bundles the libmpv runtime (libmpv-2.dll) next to the Chataigne executable
rem  so the app does not depend on mpv being installed on the machine.
rem
rem  Source order :
rem    1. %%CHATAIGNE_MPV_SRC%% (file or directory)
rem    2. dependencies_zips\Chataigne-win-x64-release-dependencies.zip
rem ===========================================================================

set "ROOT=%~dp0"
set "APP=%ROOT%%~1"
if "%~1"=="" set "APP=%ROOT%Binaries\CI\App"

set "SRC="
if defined CHATAIGNE_MPV_SRC (
    if exist "%CHATAIGNE_MPV_SRC%\libmpv-2.dll" set "SRC=%CHATAIGNE_MPV_SRC%\libmpv-2.dll"
    if exist "%CHATAIGNE_MPV_SRC%" if not defined SRC set "SRC=%CHATAIGNE_MPV_SRC%"
)
if not defined SRC if exist "%ROOT%dependencies_zips\Chataigne-win-x64-release-dependencies.zip" (
    if not exist "%APP%" mkdir "%APP%"
    tar -xOf "%ROOT%dependencies_zips\Chataigne-win-x64-release-dependencies.zip" libmpv-2.dll > "%APP%\libmpv-2.dll"
    if errorlevel 1 exit /b 1
    echo libmpv staged into %APP%
    exit /b 0
)

if not defined SRC (
    echo libmpv-2.dll source not found. Set CHATAIGNE_MPV_SRC or add the Windows dependency archive.
    exit /b 1
)

echo Staging libmpv from : %SRC%
copy /y "%SRC%" "%APP%\libmpv-2.dll" >nul
echo libmpv staged into %APP%
exit /b 0
