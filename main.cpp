#include <cstdio>
#include <cstring>
#include "MetadataSearcher.hpp"
#include "menu.hpp"
#include <filesystem>
#include <iostream>
#include "yt_dlp.hpp"
#include <ostream>
#include <string>
#include <vector>
#include "utils.hpp"
#include "tageditor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // Initial checks
    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "--help")))
    {
        printMenu();
        return 0;
    }

    if(argc < 4)
    {
        cout << "[ERROR] Expected at least 3 arguments: trax [ALBUM] [ARTIST] [URL]" << endl;
        cout << "For more information use trax -h or trax --help" << endl;
        return 1;
    }
    
    bool debug = false;
    bool single_mode = false;
    bool no_metadata_mode = false;
    bool downloaded = false;
    string albumName = string(argv[1]);
    string artistName = string(argv[2]);
    vector<string> songNames;
    string fullPath = "./" + albumName + " - " + artistName + "/";

    //Parse arguments
    for(int i = 1; i < argc; i++)
    {
        if(string(argv[i]) == "-d")
        {
            cout << "DEBUG ENABLED" << endl;
            debug = true;
        }
        else if(string(argv[i]) == "-s") 
        {
            single_mode = true;
        }
        else if(string(argv[i]) == "-n" || string(argv[i]) == "--no-meta")
        {
            no_metadata_mode = true;
        }
    }

    //Download phase
    if(single_mode)
        downloadSong(argv[3], artistName, albumName, debug);
    else 
        downloadPlaylist(argv[3], artistName, albumName, debug);

    downloaded = true;
    
    if(!no_metadata_mode)
    {
        //Retrieve album or song metadatas
        MetadataSearcher* searcher = new MetadataSearcher();
        vector<MetadataSearcher::MP3Tag>* result = searcher->searchAlbum(albumName, artistName);
        
        //Exit from program if search failed
        if(result == NULL)
        {
            cout << "Music downloaded but metadatas weren't updated" << endl;
            return 1;
        }

        // Download cover art
        searcher->downloadCoverArt(result->at(0).AlbumID);

        //Edit tags phase
        retrieveSongsNames(fullPath, &songNames);
        editTags(songNames, fullPath, result, artistName);
        
        //Clean useless files
        filesystem::remove("./"+result->at(0).AlbumID+"-front.jpg");
    }

    //End
    if(downloaded)
        cout << "Download Completed!" << endl;

    return 0;
}