#!/bin/bash

# This script automates the build and launch process.

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

# Graphite ASCII Art from README.md
GRAPHITE="
        __________               .__                __      __                     .__              
        \\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ 
         |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\
         |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/
         |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   
                \\/             \\/        \\/\\/             \\/        \\/                              
"

echo "$GRAPHITE"

echo "Checking dependencies..."
if ! command -v conan &> /dev/null; then
    echo "Error: conan is not installed. Please install it first."
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed. Please install it first."
    exit 1
fi

function build_game() {
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
}

# Always build on first run if build folder is missing
if [ ! -d "$BUILD_DIR" ]; then
    build_game
fi

while true; do
    echo ""
    echo "-----------------------------------"
    echo "   BRAINY WARRIOR - CONTROL PANEL"
    echo "-----------------------------------"
    echo "1) Run the Game"
    echo "2) Rebuild (Clean & Build)"
    echo "3) Exit"
    echo "-----------------------------------"
    read -p "Choice [1-3]: " choice

    case $choice in
        1)
            echo "Launching Brainy Warrior..."
            cd "$BUILD_DIR" && ./Brainy_Warrior
            ;;
        2)
            echo "Cleaning and Rebuilding..."
            rm -rf "$BUILD_DIR"
            build_game
            ;;
        3)
            echo "Exiting. Happy playing!"
            echo "$GRAPHITE"
            exit 0
            ;;
        *)
            echo "Invalid choice. Please select 1-3."
            ;;
    esac
done
