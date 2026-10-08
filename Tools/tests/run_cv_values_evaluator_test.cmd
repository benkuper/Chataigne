@echo off
setlocal
for /f "delims=" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set "VSROOT=%%i"
if not defined VSROOT exit /b 1
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /W4 /Fo:"%TEMP%\cv_values_evaluator_test.obj" /Fe:"%TEMP%\cv_values_evaluator_test.exe" "%~dp0cv_values_evaluator_test.cpp"
if errorlevel 1 exit /b 1
"%TEMP%\cv_values_evaluator_test.exe"
exit /b %errorlevel%
