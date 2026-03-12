#include "program.hpp"
#include "yt_dlp.hpp"
#include <cstring>
#include <iostream>

bool parseArguments(int argc, char **argv, flags_t *flag_struct, data_t* data) {
     // Initial checks
    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "--help")))
    {
        flag_struct->menu = true;        
        return true;
    }

    if(argc < 4)
    {
        std::cout << "[ERROR] Expected at least 3 arguments: trax [ALBUM] [ARTIST] [URL]" << std::endl;
        std::cout << "For more information use trax -h or trax --help" << std::endl;
        return false;
    }

    //Parse arguments
    for(int i = 1; i < argc; i++)
    {
        if(std::string(argv[i]) == "-d")
        {
            std::cout << "DEBUG ENABLED" << std::endl;
            flag_struct->debug = true;
        }
        else if(std::string(argv[i]) == "-s") 
        {
            flag_struct->singleMode = true;
        }
        else if(std::string(argv[i]) == "-n" || std::string(argv[i]) == "--no-meta")
        {
            flag_struct->noMetadataMode = true;
        }
    }

    data->albumName = std::string(argv[1]);
    data->artistName = std::string(argv[2]);
    data->url = std::string(argv[3]);
    data->fullPath = "./" + data->artistName + " - " + data->albumName + "/";

    return true;
}

bool downloadAudio(data_t data, flags_t* flags){
    int res = 0;
    if(flags->singleMode){
        res = downloadSong(data.url, flags->debug);
    }else{
        res = downloadPlaylist(data.url, data.albumName, data.artistName, flags->debug);
    }

    if(res == -1){
        return false;
    }else{
        flags.downloaded = true;
        return true;

    }
}

