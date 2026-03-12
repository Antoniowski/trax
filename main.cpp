#include "program.hpp"
#include "menu.hpp"
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    flags_t flags;
    data_t data;

    // exit if there is an error during parsing
    if(!parseArguments(argc, argv, &flags, &data)){
        endProgram(flags);
        return -1;
    }

    if(flags.menu){
        printMenu();
        return 0;
    }

    // download phase
    if(!downloadAudio(data, &flags)){
        endProgram(flags);
        return -1;
    }
    
    if(!flags.noMetadataMode){
        // retrieve album or song metadatas
        vector<string> songTitles;
        std::vector<MetadataSearcher::MP3Tag>* result = NULL;

        if(!searchMetadata(data, &flags,  &songTitles, &result)){
            endProgram(flags);
            return -1;
        }
        // edit tags
        editTagsAndCover(data, &flags, songTitles, result);        

        //clear useless files
        removeTempFiles(result);
    }

    //End
    endProgram(flags);
    return 0;
}