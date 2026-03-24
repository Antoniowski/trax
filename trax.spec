Name:           trax
Version:        0.4.4
Release:        1%{?dist}
Summary:        Music Downloader with metadata filler based on yt-dlp and MusicBrainz databases

License:        GPLv3+
URL:            https://github.com/Antoniowski/trax
Source0:        %{name}-%{version}.tar.gz
Source1:        libcoverart-master.tar.gz

BuildRequires:  cmake >= 3.15
BuildRequires:  gcc-c++
BuildRequires:  taglib-devel
BuildRequires:  libmusicbrainz5-devel
BuildRequires:  libcurl-devel
# Dipendenze di libcoverart
BuildRequires:  neon-devel
BuildRequires:  jansson-devel
BuildRequires:  libxml2-devel

Requires:       taglib
Requires:       libmusicbrainz5
Requires:       libcurl

%description
trax is a audio downloader based on yt-dlp with automatic filling of metadata tags. All info
are retrieved from MusicBrainz databases

libcoverart (metabrainz/libcoverart) will be compiled during build phase and then linked
to trax using CMAKE_PREFIX_PATH.

%prep
%autosetup -n %{name}-%{version}

mkdir -p %{_builddir}/libcoverart
tar -xzf %{SOURCE1} -C %{_builddir}/libcoverart --strip-components=1

%build
# --- Build libcoverart ---
pushd %{_builddir}/libcoverart
  cmake . \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=%{_builddir}/coverart-install
  %make_build
  make install
popd

%cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=%{_builddir}/coverart-install
%cmake_build

%install
install -Dm755 %{_vpath_builddir}/trax %{buildroot}%{_bindir}/trax

%files
%license LICENSE
%doc README.md
%{_bindir}/trax

%changelog
* Tue Mar 24 2026 Antoniowski <r97.antonio@gmail.com> 0.4.4-1
- new package built with tito

* %(date "+%a %b %d %Y") Packager Antoniowski - 0.4.4
- Initial package
