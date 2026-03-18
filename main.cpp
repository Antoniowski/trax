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

    if(flags.menu){
        printMenu();
        delete spinner;
        return 0;
    }

    if(flags.onlyMetadataMode && flags.noMetadataMode){
        std::cout << "B R U H" << std::endl;
        delete spinner;
        return 1;
    }

    if(!flags.onlyMetadataMode){
        // download phase
        if(!flags.debug){
            setupSpinner(&spinner, DOWNLOAD);
            spinner->start();
        }
        if(!downloadAudio(data, &flags)){
            spinner->stop();
            delete spinner;
            endProgram(flags);
            return -1;
        }
    }

    if(!flags.noMetadataMode){
        if(!flags.debug) setupSpinner(&spinner, METADATA_AND_COVER);
        
        if(flags.singleMode){
            // retrieve album or song metadatas
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
            // retrieve album or song metadatas
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