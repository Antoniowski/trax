#include "MetadataSearcher.hpp"
#include "program.hpp"
#include "menu.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    flags_t flags;
    data_t data;
    spinners::Spinner* spinner = new spinners::Spinner();

    // exit if there is an error during parsing
    if(!parseArguments(argc, argv, &flags, &data)){
        endProgram(flags);
        delete spinner;
        return -1;
    }

    // open manual
    if(flags.printMenu){
        printMenu();
        delete spinner;
        return 0;
    }
    
    // show version
    if(flags.printVersion){
        printVersion();
        delete spinner;
        return 0;
    }

    // handle wrong flags usage
    if(flags.skipDownload && flags.skipMetadata){
        std::cout << "B R U H" << std::endl;
        delete spinner;
        return 1;
    }

    // enable spinner if not in debug
    if(!flags.debug){
        spinner->start();
        setupSpinner(&spinner, STARTING);
    }

    // download phase
    if(!flags.skipDownload){
        if(!flags.debug) setupSpinner(&spinner, DOWNLOAD);
        if(!downloadAudio(data, &flags)){
            spinner->stop();
            delete spinner;
            endProgram(flags);
            return -1;
        }
    }

    // metadata search phase
    if(!flags.skipMetadata){
        if(!flags.debug) setupSpinner(&spinner, METADATA_AND_COVER);
        
        if(flags.singleMode){
            // Single mode metadata retrieve
            string songFileName;
            MetadataSearcher::MP3Tag* singleResult = NULL;
            if(!searchMetadata(data, &flags, &songFileName, &singleResult)){
                endProgram(flags);
                spinner->stop();
                delete spinner;
                return -1;
            }

            // edit tags
            if(!flags.debug) setupSpinner(&spinner, TAG_EDIT);
            editTagsAndCover(data, &flags, songFileName, singleResult);        
    
            //clear useless files
            if(!flags.keepImage) removeTempFiles(singleResult);

        }
        else{
            vector<string> songTitles;
            std::vector<MetadataSearcher::MP3Tag>* result = NULL;
            // Retrieve album metadatas
            if(!searchMetadata(data, &flags,  &songTitles, &result)){
                endProgram(flags);
                spinner->stop();
                delete spinner;
                return -1;
            }
    
            // edit tags
            if(!flags.debug) setupSpinner(&spinner, TAG_EDIT);
            editTagsAndCover(data, &flags, songTitles, result);        
    
            //clear useless files
            if(!flags.keepImage) removeTempFiles(result);
        }
    }

    //End
    std::cout << std::flush;
    if(!flags.debug) spinner->stop();
    endProgram(flags);
    delete spinner;
    return 0;
}