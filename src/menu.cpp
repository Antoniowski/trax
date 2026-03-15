#include "menu.hpp"
#include <iostream>

void printMenu(){
    std::cout << "Trax Manual" << std::endl;
    std::cout << "trax: trax [ALBUM] [ARTIST] [URL] [OPTIONS]" << std::endl;
    std::cout << "\tmp3 downloader based on yt-dlp and MusicBrainz database" << std::endl;
    std::cout << std::endl << "Options:" << std::endl;
    std::cout << "\t-h, --help\t\toutput trax manual" << std::endl;
    std::cout << "\t-d\t\t\tenable debug mode to see the yt-dlp output stream " << std::endl;
    std::cout << "\t-s\t\t\tuse single song mode when the inserted url doesn't refer to a playlist" << std::endl;
    std::cout << "\t-x, --no-meta\t\tavoid metadata download and application" << std::endl;
    std::cout << "\t-k, --keep-image\tkeep the cover art image" << std::endl;
    std::cout << "\t-m, --only-meta\t\tskip the download and search only the metadata" << std::endl;;
    std::cout << "\t-i, --iteration\t\tspecified which result get from the metadata query (default is 0, the first one)" << std::endl;
    std::cout << "\t-y\t\t\talbum release year" << std::endl;
}