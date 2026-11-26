#!/bin/bash

set -e
sudo -v

if command -v apt >/dev/null 2>&1; then
	echo "Packages Manager: apt"
	PKG_MAN_CMD="apt install -y"
	LIBMUSIC=libmusicbrainz5-2
	COVERART=libcoverart1
	TAG=libtag1v5
	FFMPEG=ffmpeg
elif command -v dnf >/dev/null 2>&1; then
	echo "Package Manager: dnf"
	PKG_MAN_CMD="dnf install -y"
	LIBMUSIC=libmusicbrainz5
	COVERART=""
	TAG=taglib
	FFMPEG=ffmpeg-free
elif command -v yum >/dev/null 2>&1; then
	echo "Package Manager: yum"
	PKG_MAN_CMD="yum install -y"
	LIBMUSIC=musicbrainz5
	COVERART=libcoverart
	TAG=taglib
	FFMPEG=ffmpeg
elif command -v pacman >/dev/null 2>&1; then
	echo "Package Manager: pacman"
	PKG_MAN_CMD="pacman -Sy --needed --noconfirm"
	LIBMUSIC=libmusicbrainz5
	COVERART=libcoverart
	TAG=taglib
	FFMPEG=ffmpeg
elif command -v zypper >/dev/null 2>&1; then
	echo "Package Manager: zypper"
	PKG_MAN_CMD="zypper install -y"
	LIBMUSIC=libmusicbrainz5
	COVERART=libcoverart
	TAG=taglib
	FFMPEG=ffmpeg
else
	echo "Unknown Package Manager. Operation Aborted."
	exit 1
fi

echo
echo "Installing Runtime Dependencies:"
sudo $PKG_MAN_CMD $LIBMUSIC $COVERART $TAG $FFMPEG || { echo "Dependency Installation FAILED"; exit 1; }
if [ -z "$COVERART" ]; then
    echo "NOTE: libcoverart is not available in the package repository. Please build it manually from source: https://github.com/metabrainz/libcoverart"
fi
echo "Done!"
