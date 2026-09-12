@echo off
setlocal
for /f "delims=" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set "VSROOT=%%i"
if not defined VSROOT exit /b 1
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /W4 /Fo:"%TEMP%\timecode_math_test.obj" /Fe:"%TEMP%\timecode_math_test.exe" "%~dp0timecode_math_test.cpp"
if errorlevel 1 exit /b 1
"%TEMP%\timecode_math_test.exe"
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /W4 /Fo:"%TEMP%\mtc_receiver_test.obj" /Fe:"%TEMP%\mtc_receiver_test.exe" "%~dp0mtc_receiver_test.cpp"
if errorlevel 1 exit /b 1
"%TEMP%\mtc_receiver_test.exe"
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /W4 /Fo:"%TEMP%\ltc_audio_generator_test.obj" /Fe:"%TEMP%\ltc_audio_generator_test.exe" "%~dp0ltc_audio_generator_test.cpp"
if errorlevel 1 exit /b 1
"%TEMP%\ltc_audio_generator_test.exe"
