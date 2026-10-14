@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Tools\vscode\debug.ps1" %*
exit /b %errorlevel%
