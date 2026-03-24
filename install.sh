#!/bin/bash

PROGRAM_NAME="trax"
INSTALL_DIR="/usr/local/bin"
SOURCE_DIR="./build"
SOURCE="./build/trax"

set -e

cd $SOURCE_DIR || { mkdir $SOURCE_DIR; cd $SOURCE_DIR; }
cmake .. || { echo "Error with CMake"; exit 1; }
make || { echo "Error with make"; exit 1; }
cd ..

sudo install -m 0755 $SOURCE $INSTALL_DIR/$PROGRAM_NAME 

echo "Installation Completed!"