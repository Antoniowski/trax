#include <charconv>
#include <cstdio>
#include <cstring>
#include "MetadataSearcher.hpp"
#include "menu.hpp"
#include <exception>
#include <filesystem>
#include <iostream>
#include "yt-dlp.hpp"
#include <ostream>
#include <string>
#include <vector>
#include <format>
#include "taglib/tag.h"
#include "taglib/fileref.h"

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
    //     downloadSong(argv[3], artistName, albumName, debug);
    // else 
    //     downloadPlaylist(argv[3], artistName, albumName, debug);

    //Retrieve album or song metadatas
    //MetadataSearcher* searcher = new MetadataSearcher();
    //std::vector<MetadataSearcher::MP3Tag>* result = searcher->searchAlbum(albumName, artistName);
    MetadataSearcher* searcher = new MetadataSearcher();
    std::vector<MetadataSearcher::MP3Tag>* result = searcher->searchAlbum("Moonflower", "Santana");
    
    //Exit from program if search failed
    if(result == NULL)
    {
        std::cout << "Files downloaded but metadatas weren't updated" << std::endl;
        return 1;
    }

    //Edit tags phase
    // TagLib::FileRef file("/home/antoniowski/Scaricati/Ozzy Osbourne - No More Tears (Official Audio) [mX_8p7NaibQ].mp3");
    // file.tag()->setTitle(result->Title);
    // file.tag()->setAlbum(result->Album);
    // file.tag()->setArtist(artistName);
    // file.tag()->setTrack(std::stoi((result->TrackNumber)));
    // try {
    //     file.tag()->setYear(std::stoi((result->Year).substr(0, 4)));
    // } catch (std::exception e) {
    //     file.tag()->setYear(0);condition
    // }
    // file.save();

    std::vector<std::string> songNames;
    std::string fullPath = "/home/antoniowski/Scaricati/test/";
    for(auto &entry : std::filesystem::directory_iterator(fullPath))
    {
        std::string path = entry.path().string();
        std::string dirName = "test/";
        int pos2 = path.find("test/");
        songNames.push_back(path.substr(pos2 + dirName.size()));
    }

    for(int i = 0; i < songNames.size(); i++)
    {
        TagLib::FileRef file(std::string(fullPath + songNames[i]).c_str());
        MetadataSearcher::MP3Tag tag;
        bool songFound = false;
        for(auto t : *result)
        {
            std::string fileNameStr = songNames[i];
            std::string songNameStr = t.Title;
            //lowercase
            std::transform(fileNameStr.begin(), fileNameStr.end(), fileNameStr.begin(), ::tolower);
            std::transform(songNameStr.begin(), songNameStr.end(), songNameStr.begin(), ::tolower);
            if(fileNameStr.find(songNameStr) != std::string::npos)
            {
                // found!
                std::cout << t.Title << std::endl;
                songFound = true;
                tag = t;
                break;
            }
            else 
                continue;
        }
        if (!songFound)
            continue;

        file.tag()->setTitle((tag.Title.empty() || tag.Title == " ") ? std::string("song_" + std::to_string(i)) : tag.Title);
        file.tag()->setAlbum(tag.Album);
        file.tag()->setArtist(artistName);
        try 
        {
            file.tag()->setTrack(std::stoi(tag.TrackNumber));
        } catch (std::exception e) 
        {
            file.tag()->setTrack(i);
        }
        try 
        {
            file.tag()->setYear(std::stoi((tag.Year).substr(0, 4)));
        } catch (std::exception e) 
        {
            file.tag()->setYear(0);
        }
        file.save();

        std::rename((fullPath + songNames[i]).c_str(), (fullPath + tag.Title + ".mp3").c_str());
    }

    //End
    if(downloaded)
        std::cout << "Download Completed!" << std::endl;

    return 0;
}