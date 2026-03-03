#include <cstdio>
#include <cstring>
#include "MetadataSearcher.hpp"
#include "menu.hpp"
#include <filesystem>
#include <iostream>
#include "TagEditor.hpp"
#include "yt-dlp.hpp"
#include <string>
#include <vector>

int main(int argc, char *argv[]){

    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "--help")))
    {
        printMenu();
        return 0;
    }

    if(argc < 4)
    {
        std::cout << "[ERROR] Expected at least 3 arguments: trax [ALBUM] [ARTIST] [URL]" << std::endl;
        std::cout << "For more information use trax -h or trax --help" << std::endl;
        return 1;
    }
 
    bool debug = false;
    bool single_mode = false;
    bool downloaded = false;
    std::string albumName = std::string(argv[1]);
    std::string artistName = std::string(argv[2]);

    //Parse arguments
    for(int i = 1; i < argc; i++)
    {
        if(std::string(argv[i]) == "-d")
        {
            std::cout << "DEBUG ENABLED" << std::endl;
            debug = true;
        }
        else if(std::string(argv[i]) == "-s") 
        {
            single_mode = true;
        }
    }

    // //Download phase
    // if(single_mode)
    // {
    //     downloadSong(argv[3], artistName, albumName, debug);
    // }
    // else 
    // {
    //     downloadPlaylist(argv[3], artistName, albumName, debug);
    // }

    //Retrieve album or song metadatas
    //MetadataSearcher* searcher = new MetadataSearcher();
    //std::vector<TagEditor::MP3Tag>* result = searcher->searchAlbum(albumName, artistName);
    
    //Exit from program if search failed
    //if(result == NULL)
      //  return 1;

    //Edit tags phase
    TagEditor* editor = new TagEditor();
    TagEditor::MP3Tag tag;
    FILE* mp3 = fopen("/home/antoniowski/Musica/Peal Jam - Ten/Alive.mp3", "r");
    editor->ReadMP3(mp3, tag);
    fclose(mp3);
    //End
    if(downloaded)
        std::cout << "Download Completed!" << std::endl;

    return 0;
}