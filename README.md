# Trax
<p align="center">
  <img src="./trax_img.png" alt="Trax"/>
</p>
<br/>
<br/>

A youtube audio downloader based on [yt-dlp](https://github.com/yt-dlp/yt-dlp) with automated id tag compilation with data coming form [MusicBrainz](https://musicbrainz.org/) databases. <br/>


## Dependencies
- [yt-dlp](https://github.com/yt-dlp/yt-dlp)
- [libmusicbrainz](https://musicbrainz.org/doc/libmusicbrainz) (only for manual compilation)
- [libcoverart](https://musicbrainz.org/doc/libcoverart) (only for manual compilation)

## Installation
For now the only ways to "install" Trax are compiling it using CMake or downloading an already compiled version from this GitHub page.
### CMake
To install Trax using CMake just execute the following commands while in the program directory:
>cd ./build/<br/>
>cmake .. <br/>
>make 
