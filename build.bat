@echo off
setlocal enabledelayedexpansion

set "ROOT=%~dp0"
cd /d "%ROOT%"

set "SOLUTION=Builds\VisualStudio2022\Chataigne.sln"
set "CONFIG=Release"
set "PLATFORM=x64"

set "TARGET="
set "EXTRA_ARGS="
for %%a in (%*) do (
    if /i "%%a"=="rebuild" set "TARGET=/t:Rebuild"
    if /i "%%a"=="-r" set "TARGET=/t:Rebuild"
)
for %%a in (%*) do (
    if /i not "%%a"=="rebuild" if /i not "%%a"=="-r" set "EXTRA_ARGS=!EXTRA_ARGS! %%a"
)

echo Checking for MSBuild...
set "MSBUILD="
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
    for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe"`) do (
        if not defined MSBUILD set "MSBUILD=%%i"
    )
)

if not defined MSBUILD (
    for %%p in (
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
    ) do (
        if exist %%p set "MSBUILD=%%~p"
    )
)

if not defined MSBUILD (
    echo MSBuild.exe not found. Install Visual Studio 2022 Build Tools with the MSBuild component.
    exit /b 1
)

echo Using MSBuild : %MSBUILD%
echo Solution      : %SOLUTION%
echo Configuration : %CONFIG% ^| %PLATFORM%
echo Target        : %TARGET:~3% (default: incremental build)
echo.

"%MSBUILD%" "%SOLUTION%" /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /p:PreferredToolArchitecture=x64 /m %TARGET% %EXTRA_ARGS%

if %errorlevel% NEQ 0 (
    echo.
    echo BUILD FAILED (exit code %errorlevel%)
    exit /b %errorlevel%
)

echo.
echo BUILD SUCCEEDED
if exist "Binaries\App\Chataigne.exe" echo Output : %ROOT%Binaries\App\Chataigne.exe

call "%~dp0stage_mpv.bat" "Binaries\App"
if %errorlevel% NEQ 0 exit /b %errorlevel%

exit /b 0
