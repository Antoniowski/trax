#!/bin/bash

set -e
sudo -v

if command -v apt >/dev/null 2>&1; then
	echo "Packages Manager: apt"
	PKG_MAN=apt
	LIBMUSIC=libmusicbrainz5-2
	LIBMUSIC_DEV=libmusicbrainz5-dev
	COVERART=libcoverart1
	COVERART_DEV=libcoverart-dev
	TAG=libtag1v5
	TAG_DEV=libtag1-dev
	FFMPEG=ffmpeg
elif command -v dnf >/dev/null 2>&1; then
	echo "Package Manager: dnf"
	PKG_MAN=dnf
	LIBMUSIC=libmusicbrainz5
	LIBMUSIC_DEV=libmusicbrainz5-devel
	COVERART=""
	COVERART_DEV=""
	TAG=taglib
	TAG_DEV=taglib-devel
	FFMPEG=ffmpeg-free
elif command -v yum >/dev/null 2>&1; then
	echo "Package Manager: yum"
	PKG_MAN=yum
	LIBMUSIC=musicbrainz5
	LIBMUSIC_DEV=musicbrainz5-devel
	COVERART=libcoverart
	COVERART_DEV=libcoverart-devel
	TAG=taglib
	TAG_DEV=taglib-devel
	FFMPEG=ffmpeg
elif command -v pacman >/dev/null 2>&1; then
	echo "Package Manager: pacman"
	PKG_MAN=pacman
	LIBMUSIC=libmusicbrainz5
	LIBMUSIC_DEV=libmusicbrainz5
	COVERART=libcoverart
	COVERART_DEV=libcoverart
	TAG=taglib
	TAG_DEV=taglib
	FFMPEG=ffmpeg
elif command -v zypper >/dev/null 2>&1; then
	echo "Package Manager: zypper"
	PKG_MAN=zypper
	LIBMUSIC=libmusicbrainz5
	LIBMUSIC_DEV=libmusicbrainz-devel
	COVERART=libcoverart
	COVERART_DEV=libcoverart-devel
	TAG=taglib
	TAG_DEV=taglib-devel
	FFMPEG=ffmpeg
else
	echo "Unknown Package Manager. Operation Aborted."
	exit 1
fi

echo
echo "Installing Dependencies:"
sudo $PKG_MAN install -y $LIBMUSIC $LIBMUSIC_DEV $COVERART $COVERART_DEV $TAG $TAG_DEV $FFMPEG cmake make || { echo "Dependency Installation FAILED"; exit 1; }
if [ -z "$COVERART" ]; then
    echo "NOTE: libcoverart is not available in the package repository. Please build it manually from source: https://github.com/metabrainz/libcoverart"
fi
echo "Done!"