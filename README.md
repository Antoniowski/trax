# Trax
<p align="center">
  <img src="./trax_img.png" alt="Trax"/>
</p>
<br/>
<br/>

A youtube audio downloader based on [yt-dlp](https://github.com/yt-dlp/yt-dlp) with automated id tag compilation using data from [MusicBrainz](https://musicbrainz.org/) databases. <br/>


## Dependencies
- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- [libmusicbrainz](https://musicbrainz.org/doc/libmusicbrainz) (only for manual compilation)
- [libcoverart](https://musicbrainz.org/doc/libcoverart) (only for manual compilation)

## Installation
For now the only ways to "install" Trax are compiling it using CMake or downloading an already compiled version from this GitHub page.
### CMake
To install Trax using CMake just execute the following commands while in the program directory:
>
>mkdir build <br/>
>cd build/<br/>
>cmake .. <br/>
>make <br/>
>
Doing so all the CMake generated file will be created in the build folder.
## How To Use
The basic way to use Trax is to use the following syntax: <br/> <br/>
<code>trax [ALBUM] [ARTIST] [URL]</code> <br/><br/>
This let the user to download an entire playlist and each downloaded video will be converted to mp3 files. Then the mp3 tags will be automatically filled with information retrieved using the specified album and artist. <br/>
If no info is retrieved from the MusicBrainz database, only the files download and the mp3 convertion will be performed. <br/><br/>
The user can add flags to modify the program behaviour. Each flag should be added after the URL field, with exception for "-h" flag.

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
- Add installation script
- Add support for Windows
- Add support for MacOs 