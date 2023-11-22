#!/bin/bash

# Define build directory and cache file
BUILD_DIR="./build"

# Check if the build directory exists
if [ -d "$BUILD_DIR" ]; then
    # Remove build dir
    rm -rf "$BUILD_DIR"
fi

