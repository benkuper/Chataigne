@echo off
setlocal enabledelayedexpansion

rem ===========================================================================
rem  stage_mpv.bat
rem
rem  Bundles the libmpv runtime (libmpv-2.dll) next to the Chataigne executable
rem  so the app does not depend on mpv being installed on the machine.
rem
rem  Source order :
rem    1. %%CHATAIGNE_MPV_SRC%%          path to a file (or folder containing)
rem                                      libmpv-2.dll
rem    2. %%TEMP%%\opencode\libmpv_dl\libmpv-2.dll
rem ===========================================================================

set "ROOT=%~dp0"
set "APP=%ROOT%Binaries\CI\App"

set "SRC="
if defined CHATAIGNE_MPV_SRC (
    if exist "%CHATAIGNE_MPV_SRC%\libmpv-2.dll" set "SRC=%CHATAIGNE_MPV_SRC%\libmpv-2.dll"
    if exist "%CHATAIGNE_MPV_SRC%" if not defined SRC set "SRC=%CHATAIGNE_MPV_SRC%"
)
if not defined SRC if exist "%TEMP%\opencode\libmpv_dl\libmpv-2.dll" set "SRC=%TEMP%\opencode\libmpv_dl\libmpv-2.dll"

if not defined SRC (
    echo libmpv-2.dll source not found. Set CHATAIGNE_MPV_SRC or place it in %TEMP%\opencode\libmpv_dl.
    exit /b 1
)

echo Staging libmpv from : %SRC%
copy /y "%SRC%" "%APP%\libmpv-2.dll" >nul
echo libmpv staged into %APP%
exit /b 0