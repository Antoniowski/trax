#include "menu.hpp"
#include <iostream>

void printMenu(){
    std::cout << "Trax Manual" << std::endl;
    std::cout << "trax: trax [ALBUM] [ARTIST] [URL]" << std::endl;
    std::cout << "\tmp3 downloader based on yt-dlp and MusicBrainz database" << std::endl;
    std::cout << std::endl << "Options:" << std::endl;
    std::cout << "\t-h, --help\toutput trax manual" << std::endl;
    std::cout << "\t-d\t\tenable debug mode to see the yt-dlp output stream " << std::endl;
    std::cout << "\t-s\t\tuse single song mode when the inserted url doesn't refer to a playlist" << std::endl;
}