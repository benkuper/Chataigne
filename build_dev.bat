@echo off
rem ===========================================================================
rem  build_dev.bat  - fast iterative build
rem
rem  Same as build.bat but WITHOUT whole-program optimization / LTCG, which is
rem  what makes the normal build spend 10-20 minutes in the LTCG codegen+link.
rem  Pick the fast path while iterating on code, run the real build.bat (LTCG)
rem  only for the final Release EXE.
rem ===========================================================================
setlocal enabledelayedexpansion

set "ROOT=%~dp0"
cd /d "%ROOT%"

set "SOLUTION=Builds\VisualStudio2022_CI\Chataigne.sln"
set "CONFIG=Release"
set "PLATFORM=x64"

set "TARGET="
set "EXTRA_ARGS="
for %%a in (%*) do (
    if /i "%%a"=="rebuild" set "TARGET=/t:Rebuild"
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

for %%p in (
    "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
) do if not defined MSBUILD if exist %%p set "MSBUILD=%%~p"

if not defined MSBUILD (
    echo MSBuild.exe not found.
    exit /b 1
)

echo Using MSBuild : %MSBUILD%
echo Configuration : %CONFIG% ^| %PLATFORM%  (no LTCG - fast dev build)
echo Target        : %TARGET:~3% (default: incremental build)
echo.

"%MSBUILD%" "%SOLUTION%" /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /p:PreferredToolArchitecture=x64 /p:WholeProgramOptimization=false /p:LinkTimeCodeGeneration=Default /m %TARGET% %EXTRA_ARGS%

if %errorlevel% NEQ 0 (
    echo.
    echo BUILD FAILED (exit code %errorlevel%)
    exit /b %errorlevel%
)

echo.
echo BUILD SUCCEEDED
if exist "Binaries\CI\App\Chataigne.exe" echo Output : %ROOT%Binaries\CI\App\Chataigne.exe

call "%~dp0stage_mpv.bat"

exit /b 0
