@echo off
REM Usage: tools\run-build-validator.bat [buildDir] [config] [pluginName]
set BUILD_DIR=%1
if "%BUILD_DIR%"=="" set BUILD_DIR=build
set CONFIG=%2
if "%CONFIG%"=="" set CONFIG=RelWithDebInfo
set PLUGIN=%3
if "%PLUGIN%"=="" set PLUGIN=SpectralCanvasPro

powershell -ExecutionPolicy Bypass -File "%~dp0build-validator.ps1" -BuildDir "%BUILD_DIR%" -Config "%CONFIG%" -PluginName "%PLUGIN%"
exit /b %errorlevel%