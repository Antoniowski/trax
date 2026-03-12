#include <cstdio>
#include <cstring>
#include "program.hpp"
#include "MetadataSearcher.hpp"
#include "menu.hpp"
#include <exception>
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
    flags_t flags;
    data_t data;

    // Exit if there is an error during parsing
    if(!parseArguments(argc, argv, &flags, &data)){
        return 1;
    }

    if(flags.menu){
        printMenu();
        return 0;
    }

    vector<string> songNames;
    //Download phase
    if(!downloadAudio(data, &flags)){
        cout << "[ERROR] There was a problem during audio download" << endl;
        return 1;
    }
    
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
        try 
        {
            searcher->downloadCoverArt(result->at(0).AlbumID);
        } 
        catch (exception e) 
        {
            cout << "Error in cover art download or cover art not found in database" << endl;
        }

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