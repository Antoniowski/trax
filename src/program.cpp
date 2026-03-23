#include "program.hpp"
#include "version.hpp"
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
    if (argc == 2 && (std::string(argv[1]) == "-h") || std::string(argv[1]) ==  "--help"){
        flag_struct->printMenu = true;        
        return true;
    }

    if (argc == 2 && (std::string(argv[1]) ==  "-v")){
        flag_struct->printVersion = true;        
        return true;
    }

    if(argc < 4){
        std::cout << "[ERROR] Expected at least 3 arguments: trax [ALBUM] [ARTIST] [URL]" << std::endl;
        std::cout << "For more information use trax -h or trax --help" << std::endl;
        flag_struct->pError = true;
        return false;
    }

    //Parse arguments
    for(int i = 1; i < argc; i++){
        // debug
        if(std::string(argv[i]) == "-d"){
            std::cout << "DEBUG ENABLED" << std::endl;
            flag_struct->debug = true;
        }

        // single song mode
        else if(std::string(argv[i]) == "-s") {
            flag_struct->singleMode = true;
            data->songName = argv[i + 1];
            if(data->songName.empty() || data->songName == ""){
                std::cout << "[ERROR] Specify a valid song name" << std::endl;
                flag_struct->pError = true;
                return false;
            }
        }

        // skip metadata
        else if(std::string(argv[i]) == "-n" || std::string(argv[i]) == "--no-meta" || std::string(argv[1])=="" || std::string(argv[2])=="" || std::string(argv[1])==" " || std::string(argv[2])==" "){
            flag_struct->skipMetadata = true;
        }

        // keep cover image
        else if(std::string(argv[i]) == "-k" || std::string(argv[i]) == "--keep-image"){
            flag_struct->keepImage = true;
        }

        // iteration selection
        else if(std::string(argv[i]) == "-i" || std::string(argv[i]) == "--iteration"){
            try{
                flag_struct->iteration = std::stoi(argv[i+1]);
            }
            catch(const std::invalid_argument &e){
                std::cout << "[ERROR] Use a valid value for the -i flag" << std::endl;
                flag_struct->pError = true;
                return false;
            }
            catch(const std::out_of_range &e){
                std::cout << "[ERROR] Value used for -i flag is out of range" << std::endl;
                flag_struct->pError = true;
                return false;
            }
        }

        // skip download
        else if(std::string(argv[i]) == "-m" || std::string(argv[i]) == "--only-meta"){
            flag_struct->skipDownload = true;
        }

        // specify year
        else if(std::string(argv[i]) == "-y"){
            try{
                data->year = std::stoi(argv[i+1]);
            }
            catch(const std::invalid_argument &e){
                std::cout << "[ERROR] Use a valid value for the -y flag" << std::endl;
                flag_struct->pError = true;
                return false;
            }
            catch(const std::out_of_range &e){
                std::cout << "[ERROR] Value used for -y flag is out of range" << std::endl;
                flag_struct->pError = true;
                return false;
            }
        }

        // format selection
        else if(std::string(argv[i]) == "-f"){
            flag_struct->format = std::string(argv[i+1]);
            
            if(
            flag_struct->format != "mp3" &&
            flag_struct->format != "aac" &&
            flag_struct->format != "m4a" &&
            flag_struct->format != "alac" &&
            flag_struct->format != "flac" &&
            flag_struct->format != "opus" &&
            flag_struct->format != "vorbis" &&
            flag_struct->format != "wav"){
                std::cout << "[ERROR] File format " + flag_struct->format + " not supported" << std::endl;
                flag_struct->pError = true;
                return false;
            }
        }

        // skip image download
        else if(std::string(argv[i]) == "--no-image"){
            flag_struct->skipImageDownload = true;
        }

        // enable complex file name
        else if(std::string(argv[i]) == "-N"){
            flag_struct->complexName = true;
        }

        // add genres
        else if(std::string(argv[i]) == "-g"){
            flag_struct->addGenres = true;
            flag_struct->addRawGenres = false;
            data->genres = std::string(argv[i+1]);
        }

        // add raw genres
        else if(std::string(argv[i]) == "-rg"){
            flag_struct->addGenres = false;
            flag_struct->addRawGenres = true;
            data->genres = std::string(argv[i+1]);
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
        res = downloadSong(data.url, flags->format, flags->debug);
    }
    else{
        res = downloadPlaylist(data.url, data.albumName, data.artistName, flags->format, flags->debug);
    }

    if(res == -1){
        std::cout << "[ERROR] There was a problem during audio download" << std::endl;
        flags->pError = true;
        return false;
    }
    else{
        flags->pAudioDownloaded = true;
        return true;

    }
}

bool searchMetadata(data_t data, flags_t* flags, std::vector<std::string>* titles, std::vector<MetadataSearcher::MP3Tag>** metadata){

    //Check if folder with audio files exists
    if(!std::filesystem::exists(data.fullPath) || std::filesystem::is_empty(data.fullPath)){
        std::cout << "[ERROR] There is no directory containing audio files in the current path or the directory is empty" << std::endl;
        flags->pError = true;
        return false;
    }

    //Retrieve album or song metadatas
    MetadataSearcher* searcher = new MetadataSearcher();

    if(flags->debug){
        *metadata = searcher->searchAlbum(data.albumName, data.artistName, data.year, flags->iteration);
    }
    else{
        OutputSuppressor suppress;
        *metadata = searcher->searchAlbum(data.albumName, data.artistName, data.year, flags->iteration);
    }
    
    //Exit from program if search failed
    if(*metadata == NULL){
        std::cout << "[ERROR] Music downloaded but metadata weren't updated" << std::endl;
        flags->pError = true;
        delete searcher;
        return false;
    }

    // Download cover art
    if(!flags->skipImageDownload)
    {
        try{
            searcher->downloadCoverArt((*metadata)->at(0).AlbumID);
            flags->pCoverDownloaded = true;
        }
        catch (std::exception e){
            std::cout << "[WARNING] There was a problem while downloading the cover art or cover art was not found in database" << std::endl;
            flags->pCoverDownloaded = false;
        }
    }

    //Edit tags phase
    retrieveSongsNames(data.fullPath, titles);
    delete searcher;
    return true;
}


bool searchMetadata(data_t data, flags_t* flags, std::string* songFileName, MetadataSearcher::MP3Tag** metadata){
    //Retrieve album or song metadatas
    MetadataSearcher* searcher = new MetadataSearcher();
    if(flags->debug){
        *metadata = searcher->searchSong(data.songName, data.albumName, data.artistName, data.year, flags->iteration);
    }else{
        OutputSuppressor suppress;
        *metadata = searcher->searchSong(data.songName, data.albumName, data.artistName, data.year, flags->iteration);
    }
    
    //Exit from program if search failed
    if(*metadata == NULL){
        std::cout << "[ERROR] Music downloaded but metadata weren't updated" << std::endl;
        flags->pError = true;
        delete searcher;
        return false;
    }

    // Download cover art
    if(!flags->skipImageDownload){
        try{
            searcher->downloadCoverArt((*metadata)->AlbumID);
            flags->pCoverDownloaded = true;
        }
        catch (std::exception e){
            std::cout << "[WARNING] There was a problem while downloading the cover art or cover art was not found in database" << std::endl;
            flags->pCoverDownloaded = false;
        }
    }

    //Edit tags phase
    retrieveSingleFileSongName(data.songName, songFileName);
    delete searcher;
    return true;
}


void editTagsAndCover(data_t data, flags_t* flags, std::vector<std::string> titles, std::vector<MetadataSearcher::MP3Tag> *metadata){
    editTags(titles, data.fullPath, metadata, data.artistName, data.genres, flags->addRawGenres, flags->skipImageDownload, flags->complexName);
    flags->pTagEdited = true;
}


void editTagsAndCover(data_t data, flags_t* flags, std::string songFileName, MetadataSearcher::MP3Tag *metadata){
    editTag(songFileName, metadata, data.artistName, data.genres, flags->addRawGenres, flags->skipImageDownload, flags->complexName);
    flags->pTagEdited = true;
}


void removeTempFiles(std::vector<MetadataSearcher::MP3Tag>* metadata){
    std::filesystem::remove("./"+metadata->at(0).AlbumID+"-front.jpg");
}


void removeTempFiles(MetadataSearcher::MP3Tag* metadata){
    std::filesystem::remove("./"+metadata->AlbumID+"-front.jpg");
}


void endProgram(flags_t flags){
    if(flags.pError && !flags.pAudioDownloaded && !flags.pCoverDownloaded && !flags.pTagEdited)
        return;

    std::cout << "FINISHED!" << std::endl;
    std::cout << "AUDIO DOWNLOAD: ";
    std::cout << (flags.pAudioDownloaded ? "[OK]" : "[FAILED]") << std::endl;
    std::cout << "COVER ART DOWNLOAD: ";
    std::cout << (flags.pCoverDownloaded ? "[OK]" : "[FAILED]") << std::endl;
    std::cout << "TAG EDIT: ";
    std::cout << (flags.pTagEdited ? "[OK]" : "[FAILED]") << std::endl;
}


void setupSpinner(spinners::Spinner** spinner, Phases currentPhase){
    switch (currentPhase) {
        case STARTING:
            (*spinner)->setText("Starting");
            break;
        case DOWNLOAD:
            (*spinner)->setText("Downloading Audio");
            break;
        case METADATA_AND_COVER:
            (*spinner)->setText("Looking For Metadata And Cover");
            break;
        case TAG_EDIT:
            (*spinner)->setText("Editing Tags");
            break;
        default:
            break;
    }
    (*spinner)->setInterval(100);
    (*spinner)->setSymbols("dots4");
}


void printVersion(){
    std::cout << "Trax " + std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(REVISION_VERSION) << std::endl;
}