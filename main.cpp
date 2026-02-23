#include <cstring>
#include "menu.hpp"
#include <iostream>
#include <filesystem>
#include "TagEditor.hpp"
#include "yt-dlp.hpp"
#include <musicbrainz5/Metadata.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/Artist.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/NameCreditList.h>
#include <string>

using namespace MusicBrainz5;

int main(int argc, char *argv[]){

    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "--help"))) {
        printMenu();
        return 0;
    }

    if(argc < 4)
    {
        std::cout << "[ERROR] Expected at least 3 arguments: trax [ARTIST] [ALBUM] [URL]" << std::endl;
        std::cout << "For more information use trax -h or trax --help" << std::endl;
        return 1;
    }
 
    bool debug = false;
    bool single_mode = false;

    for(int i = 1; i < argc; i++){
        if(std::string(argv[i]) == "-d"){
            std::cout << "DEBUG ENABLED" << std::endl;
            debug = true;
        }else if(std::string(argv[i]) == "-s") {
            single_mode = true;
        }
    }

    if(single_mode){
        downloadSong(argv[3], debug);
    }else {
        downloadPlaylist(argv[3], debug);
    }

    std::cout << "Download Completed!" << std::endl;

    return 0;
}