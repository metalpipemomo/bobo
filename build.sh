#!/bin/bash

# Update git submodules recursively
git submodule update --init --recursive

# Check if there was an error with the git command
if [ $? -ne 0 ]; then
    echo "Error: Failed to update git submodules."
    exit 1
fi

# Run the clear_cmake_cache.sh script
./clean.sh

# Check if there was an error with the clear script
if [ $? -ne 0 ]; then
    echo "Error: Failed to clear CMake cache."
    exit 1
fi

# Navigate to the build directory or create it if it doesn't exist
BUILD_DIR="./build"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Run cmake
cmake ..

# Check if there was an error with the cmake command
if [ $? -ne 0 ]; then
    echo "Error: cmake command failed."
    exit 1
fi

# Go back to the root directory and run the run_make.sh script
cd ..
./run.sh

# Check if there was an error with the run script
if [ $? -ne 0 ]; then
    echo "Error: Failed to run make."
    exit 1
fi

