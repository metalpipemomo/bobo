@echo off

REM Update git submodules recursively
git submodule update --init --recursive

REM Check if there was an error with the git command
if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to update git submodules.
    exit /b 1
)

REM Run the clean.bat script
call clean.bat

REM Check if there was an error with the clear script
if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to clear CMake cache.
    exit /b 1
)

REM Navigate to the build directory or create it if it doesn't exist
SET BUILD_DIR=./build

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

cd "%BUILD_DIR%"

REM Run cmake
cmake ..

REM Check if there was an error with the cmake command
if %ERRORLEVEL% NEQ 0 (
    echo Error: cmake command failed.
    exit /b 1
)

REM Go back to the root directory and run the run.bat script
cd ..
call run.bat

REM Check if there was an error with the run script
if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to run the executable.
    exit /b 1
)
