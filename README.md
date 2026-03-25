# Trax
<p align="center">
  <img src="./trax_img.png" alt="Trax"/>
</p>
<br/>
<br/>
<p align="center">
  <a href="https://ko-fi.com/P5P11VSMXF"><img src="https://ko-fi.com/img/githubbutton_sm.svg"/></a>
</p>

An audio downloader based on [yt-dlp](https://github.com/yt-dlp/yt-dlp) with automated id tag compilation using data from [MusicBrainz](https://musicbrainz.org/) databases. <br/>

## Dependencies
- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- [libmusicbrainz](https://musicbrainz.org/doc/libmusicbrainz) 
- [libcoverart](https://musicbrainz.org/doc/libcoverart)
- [TagLib](https://taglib.org/)
- libmusicbrainz5-devel (needed for compilation)
- libcoverart-devel (needded for compilation)
- TagLib-devel (needed for compilation)

>[!TIP]
>If you are using a Debian based distro like Ubuntu, download yt-dlp from the official GitHub repo to avoid compatibility problems
>
<br/>

## Installation

### Linux
To install Trax you can downloading an already compiled version from this GitHub repo or can use the <code>install.sh</code> script contained in the repo.

#### Using Install.sh
>[!NOTE]
>This method works with Debian based distros, Fedora, Arch Linux and OpenSUSE. <br/>
>If your distro is not in the list you can still install the program by manually installing dependencies and then executing the <code>build.sh</code> and <code>simple_install.sh</code> scripts.

First, get a copy of the source code.
You can either clone the repo or get it from the latest release using:
```
curl -L https://github.com/Antoniowski/trax/archive/refs/tags/v0.4.4.tar.gz -o ./trax.tar.gz
tar -xzvf ./trax.tar.gz
```
Then move in the root directory and execute the <code>install.sh</code> script:
```
./install.sh
```
This script will handle the dependencies download and installation, the program compilation using CMake and the actual installation of the program on your system.
<br/><br/>
#### Using A Compiled Version
This is the easiest way to get Trax. Just download one of the [releases](https://github.com/Antoniowski/trax/releases) or get the latest one using the following command: <br/>
```
curl -L https://github.com/Antoniowski/trax/releases/download/v0.4.4/trax_linux_x86_64 -o ./trax
```
When the download is finished, run the following command to install the program on your system:
```
sudo install -m 0755 ./trax /usr/bin/trax 
```

or use the <code>simple_install.sh</code> script in the same folder where you put the executable. You can find the script as asset in each [release](https://github.com/Antoniowski/trax/releases).
>[!IMPORTANT]
>You still need all the dependencies to run trax. You can install them using the <code>install_runtime_dependencies.sh</code> script. You can found it attached at each [release](https://github.com/Antoniowski/trax/releases) as asset

### Windows and MacOS
Coming soon...
<br/>
<br/>

## How To Use
The basic way to use Trax is to use the following syntax: <br/> <br/>
<code>trax [ALBUM] [ARTIST] [URL]</code> <br/><br/>
This let the user to download an entire playlist and each downloaded video will be converted to the desired format (default is mp3). Then the new file tags will be automatically filled with information retrieved using the specified album and artist. <br/>
If no info is retrieved from the MusicBrainz database, only the files download and the mp3 convertion will be performed. <br/><br/>
The user can add flags to modify the program behaviour. Each flag should be added after the URL field, with exception for "-h" flag.

<code>trax [ALBUM] [ARTIST] [URL] [OPTIONS]</code> <br/>

#### Formats
Output files can be converted to various formats using the <code>-f</code> flag. The available formats are:
- mp3 (default)
- aac
- m4a
- alac
- flac
- wav
- opus
- vorbis

#### Flags
| Flag | Description |
| ---- | ----------- |
| -h, --help | output trax manual |
| -d | enable debug mode to see the yt-dlp output stream |
| -v | print current program version |
| -s | -s [SONG NAME] use single song mode when the inserted url doesn't refer to a playlist |
| -x, --no-meta | avoid metadata download and file manipulation; just download audio |
| -k, --keep-image | keep the cover art image |
| -m, --only-meta | skip the download and search only the metadata |
| -i, --iteration | -i [VALUE] specify which result to get from the metadata query (default is 0, the first one) |
| -y | -y [YEAR] specify album release year |
| -f | -f [FORMAT] used to specify the audio format of output files. Default is mp3. Other available formats are: aac, m4a, flac, alac, wav, opus, vorbis |
| --no-image | skip image download and application |
| -N | enable complete naming for files |
| -g | -g [GENRES] used to add geners to files. To add more genres use the "/" as separator (e.g. rock/pop) |
| -rg | -rg [GENRES] used to add geners to files as raw string |




## Workarounds
Sometimes the program could fail because the metadata retrieved from MusicBrainz database are broken or incomplete. To retry a metadata research you can use the <code>-m</code> flag to skip the download phase.<br/> If errors still occur try also to add a specific year for the album using the <code>-y</code> flag and try to use the <code>-i</code> flag to force the program to retrieve a different result from the query results list.

## Other Info
The code also contains the C++ [spinner header](https://github.com/jkuri/spinners) created by [jkuri](https://github.com/jkuri)
