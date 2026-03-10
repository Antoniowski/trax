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
cp $SOURCE $INSTALL_DIR/$PROGRAM_NAME
chmod +x $INSTALL_DIR/$PROGRAM_NAME

