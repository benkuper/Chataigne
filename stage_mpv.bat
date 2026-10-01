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
rem    2. Binaries\mpv-win-x64 (locally built playback runtime)
rem    3. dependencies_zips\Chataigne-win-x64-release-dependencies.zip
rem ===========================================================================

set "ROOT=%~dp0"
set "APP=%ROOT%%~1"
if "%~1"=="" set "APP=%ROOT%Binaries\CI\App"

set "SRC="
if defined CHATAIGNE_MPV_SRC (
    if exist "%CHATAIGNE_MPV_SRC%\libmpv-2.dll" set "SRC=%CHATAIGNE_MPV_SRC%\libmpv-2.dll"
    if exist "%CHATAIGNE_MPV_SRC%" if not defined SRC set "SRC=%CHATAIGNE_MPV_SRC%"
)
if not defined SRC if exist "%ROOT%Binaries\mpv-win-x64\libmpv-2.dll" set "SRC=%ROOT%Binaries\mpv-win-x64\libmpv-2.dll"
if not defined SRC if exist "%ROOT%dependencies_zips\Chataigne-win-x64-release-dependencies.zip" (
    if not exist "%APP%" mkdir "%APP%"
    tar -xOf "%ROOT%dependencies_zips\Chataigne-win-x64-release-dependencies.zip" libmpv-2.dll > "%APP%\libmpv-2.dll"
    if errorlevel 1 exit /b 1
    tar -tf "%ROOT%dependencies_zips\Chataigne-win-x64-release-dependencies.zip" | findstr /b /c:"mpv-licenses/" >nul
    if not errorlevel 1 (
        tar -xf "%ROOT%dependencies_zips\Chataigne-win-x64-release-dependencies.zip" -C "%APP%" mpv-licenses
        if errorlevel 1 exit /b 1
    )
    echo libmpv staged into %APP%
    exit /b 0
)

if not defined SRC (
    echo libmpv-2.dll source not found. Set CHATAIGNE_MPV_SRC or add the Windows dependency archive.
    exit /b 1
)

echo Staging libmpv from : %SRC%
if not exist "%APP%" mkdir "%APP%"
copy /y "%SRC%" "%APP%\libmpv-2.dll" >nul
if errorlevel 1 exit /b 1
for %%f in ("%SRC%") do set "NOTICES=%%~dpfmpv-licenses"
if exist "%NOTICES%" (
    xcopy /e /i /y "%NOTICES%" "%APP%\mpv-licenses" >nul
    if errorlevel 1 exit /b 1
)
echo libmpv staged into %APP%
exit /b 0
