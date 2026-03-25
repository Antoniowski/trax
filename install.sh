#!/bin/bash

PROGRAM_NAME="trax"
INSTALL_DIR="/usr/bin"
SOURCE_DIR="./build"
SOURCE="./build/trax"

BUILD_SCRIPT="./build.sh"
DEPENDENCIES_SCRIPT="./install_dependencies.sh"

set -e
sudo -v

cat << "EOF"
  _____ ____      _    __  __  ___ _   _ ____ _____  _    _     _     _____ ____  
 |_   _|  _ \    / \   \ \/ / |_ _| \ | / ___|_   _|/ \  | |   | |   | ____|  _ \ 
   | | | |_) |  / _ \   \  /   | ||  \| \___ \ | | / _ \ | |   | |   |  _| | |_) |
   | | |  _ <  / ___ \  /  \   | || |\  |___) || |/ ___ \| |___| |___| |___|  _ < 
   |_| |_| \_\/_/   \_\/_/\_\ |___|_| \_|____/ |_/_/   \_\_____|_____|_____|_| \_\
EOF
                                                          
echo
if [ "$(uname -s)" = "Linux" ]; then
	echo "OS: Supported!"
else
	echo "OS: Not supported! Aborting operation."
	exit 1
fi

$DEPENDENCIES_SCRIPT
$BUILD_SCRIPT

echo
echo "Installing Program..."
sudo install -m 0755 ./$PROGRAM_NAME $INSTALL_DIR/$PROGRAM_NAME
rm ./$PROGRAM_NAME
echo "Done!"
echo
echo "INSTALLATION COMPLETED!"