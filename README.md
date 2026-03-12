# Trax
<p align="center">
  <img src="./trax_img.png" alt="Trax"/>
</p>
<br/>
<br/>
<p align="center">
  <a href="https://ko-fi.com/P5P11VSMXF"><img src="https://ko-fi.com/img/githubbutton_sm.svg"/></a>
</p>

A youtube audio downloader based on [yt-dlp](https://github.com/yt-dlp/yt-dlp) with automated id tag compilation using data from [MusicBrainz](https://musicbrainz.org/) databases. <br/>

>
> #### DISCLAIMER
> The program at the moment <b>SHOULD</b> work without problems but it is still far to be perfect. <br/>
>

## Dependencies
- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- [libmusicbrainz](https://musicbrainz.org/doc/libmusicbrainz) (only for manual compilation)
- [libcoverart](https://musicbrainz.org/doc/libcoverart) (only for manual compilation)

## Installation
To "install" Trax you can compiling it using CMake, downloading an already compiled version from this GitHub page or using the <code>install.sh</code> script (only Linux for now).
### CMake
To install Trax using CMake just execute the following commands while in the program directory:


>mkdir build <br/>
>cd build/<br/>
>cmake .. <br/>
>make <br/>

Doing so all the CMake generated file will be created in the build folder.

### Install.sh
After cloning the repo, you can install Trax by using the following command: <br/>
>
> sudo ./install.sh
>

This will still compile the code using CMake but it will also add the program
to the <code>/usr/local/bin</code> directory.

### Use A Compiled Version
This is the easiest way to "install" Trax. Just download one of the [releases](https://github.com/Antoniowski/trax/releases). If you want to use trax globally you can use the <code>simple_install.sh</code> provided with each release. The script will simply move the executable in the <code>/usr/local/bin</code> folder.

## How To Use
The basic way to use Trax is to use the following syntax: <br/> <br/>
<code>trax [ALBUM] [ARTIST] [URL]</code> <br/><br/>
This let the user to download an entire playlist and each downloaded video will be converted to mp3 files. Then the mp3 tags will be automatically filled with information retrieved using the specified album and artist. <br/>
If no info is retrieved from the MusicBrainz database, only the files download and the mp3 convertion will be performed. <br/><br/>
The user can add flags to modify the program behaviour. Each flag should be added after the URL field, with exception for "-h" flag.

<code>trax [ALBUM] [ARTIST] [URL] [OPTIONS]</code> <br/>

### Flags
| Flag | Description |
| ---- | ----------- |
| -h, --help | output trax manual |
| -d | enable debug mode to see the yt-dlp output stream |
| -s | use single song mode when the inserted url doesn't refer to a playlist |
| -x, --no-meta | avoid metadata download and file manipulation; just download audio |

## Other Info
The code also contains the C++ [spinner header](https://github.com/jkuri/spinners) created by [jkuri](https://github.com/jkuri)

## Supported Platforms
- Linux
- Windows 11 [TODO]
- MacOs [TODO]

## TODO List
- ~~Suppress unwanted warning coming from MusicBrainz library~~
- ~~Add installation script~~ [DONE]
- Add support for Windows
- Add support for MacOs 
- ~~Fix problem when Cover Art is not found~~ [DONE]