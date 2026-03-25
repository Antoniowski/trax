#!/bin/bash
ROGRAM_NAME="trax"
SOURCE="./build/trax"

set -e

sudo -v

echo
echo "Compiling Program:"
cmake -B build || { echo "Error during CMake command"; exit 1; }
cmake --build build || { echo "Program Compilation FAILED"; exit 1; }
mv $SOURCE ./$PROGRAM_NAME
echo "Done!"