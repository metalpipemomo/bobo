@echo off

REM Define build directory and output directory (assuming Release configuration for this example)
SET BUILD_DIR=./build

REM Navigate to the build directory containing the built executable
cd "%BUILD_DIR%"

REM Get the full path of the directory where the batch script is located
FOR %%i IN ("%~dp0.") DO SET SCRIPT_DIR=%%~ni

REM Print the name of the directory where the batch script is located
echo The script is located in: %SCRIPT_DIR%

REM Execute the main project executable
start %SCRIPT_DIR%.sln

REM Check if there was an error running the executable
if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to open solution!
    exit /b 1
)
