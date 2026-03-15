#include "program.hpp"
#include "yt_dlp.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "MetadataSearcher.hpp"
#include "utils.hpp"
#include "tageditor.hpp"
#include <filesystem>
#include "OutputSuppressor.hpp"

bool parseArguments(int argc, char **argv, flags_t *flag_struct, data_t* data) {
     // Initial checks
    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "--help"))){
        flag_struct->menu = true;        
        return true;
    }

    if(argc < 4){
        std::cout << "[ERROR] Expected at least 3 arguments: trax [ALBUM] [ARTIST] [URL]" << std::endl;
        std::cout << "For more information use trax -h or trax --help" << std::endl;
        return false;
    }

    //Parse arguments
    for(int i = 1; i < argc; i++){
        if(std::string(argv[i]) == "-d"){
            std::cout << "DEBUG ENABLED" << std::endl;
            flag_struct->debug = true;
        }
        else if(std::string(argv[i]) == "-s") {
            flag_struct->singleMode = true;
        }
        else if(std::string(argv[i]) == "-n" || std::string(argv[i]) == "--no-meta"){
            flag_struct->noMetadataMode = true;
        }
        else if(std::string(argv[i]) == "-k" || std::string(argv[i]) == "--keep-image"){
            flag_struct->keepImage = true;
        }else if(std::string(argv[i]) == "-i" || std::string(argv[i]) == "--iteration"){
            try{
                flag_struct->iteration = std::stoi(argv[i+1]);
            }catch(const std::invalid_argument &e){
                std::cout << "[ERROR] Use a valid value for the -i flag" << std::endl;
                return false;
            }catch(const std::out_of_range &e){
                std::cout << "[ERROR] Value used for -i flag is out of range" << std::endl;
                return false;
            }
        }else if(std::string(argv[i]) == "-m" || std::string(argv[i]) == "--only-meta"){
            flag_struct->onlyMetadataMode = true;
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
        std::cout << "[ERROR] There was a problem during audio download" << std::endl;
        return false;
    }else{
        flags->pAudioDownloaded = true;
        return true;

    }
}

bool searchMetadata(data_t data, flags_t* flags, std::vector<std::string>* titles, std::vector<MetadataSearcher::MP3Tag>** metadata){
    //Retrieve album or song metadatas
    MetadataSearcher* searcher = new MetadataSearcher();

    if(flags->debug){
        *metadata = searcher->searchAlbum(data.albumName, data.artistName);
    }else{
        OutputSuppressor suppress;
        *metadata = searcher->searchAlbum(data.albumName, data.artistName);
    }
    
    //Exit from program if search failed
    if(*metadata == NULL){
        std::cout << "[ERROR] Music downloaded but metadata weren't updated" << std::endl;
        delete searcher;
        return false;
    }

    // Download cover art
    try{
        searcher->downloadCoverArt((*metadata)->at(0).AlbumID);
        flags->pCoverDownloaded = true;
    }
    catch (std::exception e){
        std::cout << "[WARNING] There was a problem while downloading the cover art or cover art was not found in database" << std::endl;
        flags->pCoverDownloaded = false;
    }

    //Edit tags phase
    retrieveSongsNames(data.fullPath, titles);
    delete searcher;
    return true;
}

void editTagsAndCover(data_t data, flags_t* flags, std::vector<std::string> titles, std::vector<MetadataSearcher::MP3Tag> *metadata){
    editTags(titles, data.fullPath, metadata, data.artistName);
    flags->pTagEdited = true;
}

void removeTempFiles(std::vector<MetadataSearcher::MP3Tag>* metadata){
    std::filesystem::remove("./"+metadata->at(0).AlbumID+"-front.jpg");
}

void endProgram(flags_t flags){
    std::cout << "FINISHED!" << std::endl;
    std::cout << "AUDIO DOWNLOAD: ";
    std::cout << (flags.pAudioDownloaded ? "[OK]" : "[FAILED]") << std::endl;
    std::cout << "COVER ART DOWNLOAD: ";
    std::cout << (flags.pCoverDownloaded ? "[OK]" : "[FAILED]") << std::endl;
    std::cout << "TAG EDIT: ";
    std::cout << (flags.pTagEdited ? "[OK]" : "[FAILED]") << std::endl;
}