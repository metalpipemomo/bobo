#!/bin/bash

# Define build directory
BUILD_DIR="./build"

# Check if the build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: $BUILD_DIR directory does not exist."
    exit 1
fi

# Navigate to the build directory
cd "$BUILD_DIR"

# Run the make command
make

DIR_NAME=$(basename $(dirname $(pwd)))

if [[ -f "./$DIR_NAME" ]]; then
    ./$DIR_NAME "$@"
else
    echo "Executable $DIR_NAME not found!"
    exit 1
fi
