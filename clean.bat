@echo off

REM Define build directory
SET BUILD_DIR=./build

REM Check if the build directory exists
if exist "%BUILD_DIR%" (
    REM Remove build dir
    rmdir /s /q "%BUILD_DIR%"
)
