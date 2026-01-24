#!/bin/bash

# This script automates the build and launch process.

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

echo "Checking dependencies..."
if ! command -v conan &> /dev/null; then
    echo "Error: conan is not installed. Please install it first."
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed. Please install it first."
    exit 1
fi

# Initialize build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

cd "$PROJECT_DIR"

# Install dependencies using Conan
echo "Installing dependencies with Conan..."
conan install . --output-folder=build --build=missing

if [ $? -ne 0 ]; then
    echo "Error: Conan installation failed."
    exit 1
fi

# Configure and build the project
echo "Configuring and building the project..."
cd "$BUILD_DIR"
cmake .. -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed."
    exit 1
fi

cmake --build .

if [ $? -ne 0 ]; then
    echo "Error: Build failed."
    exit 1
fi

# Launch the game
echo "Launching Brainy Warrior..."
./Brainy_Warrior
