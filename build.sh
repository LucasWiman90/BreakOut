#!/bin/bash

# Define the build directory
BUILD_DIR="build"

# Clean the build directory if it exists
if [ -d "$BUILD_DIR" ]; then
    rm -rf $BUILD_DIR/*
else
    mkdir $BUILD_DIR
fi

# Navigate to the build directory
cd $BUILD_DIR

# Run CMake configuration
cmake -DCMAKE_BUILD_TYPE=Debug ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    exit 1
fi

# Build the project
cmake --build .
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
fi

echo "Build successful! Run with ./$BUILD_DIR/bin/breakout"

# Return to the project root directory
cd ..

# Execute the program
#./build/Breakout