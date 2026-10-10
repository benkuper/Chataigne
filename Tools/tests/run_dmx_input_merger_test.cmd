@echo off
setlocal
for /f "delims=" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set "VSROOT=%%i"
if not defined VSROOT exit /b 1
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /W4 /Fo:"%TEMP%\dmx_input_merger_test.obj" /Fe:"%TEMP%\dmx_input_merger_test.exe" "%~dp0dmx_input_merger_test.cpp"
if errorlevel 1 exit /b 1
"%TEMP%\dmx_input_merger_test.exe"
exit /b %errorlevel%
