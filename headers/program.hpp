#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "MetadataSearcher.hpp"
#include <string>
#include <vector>

typedef struct flags{
    bool menu = false;
    bool debug = false;
    bool singleMode = false;
    bool noMetadataMode = false;
    bool pAudioDownloaded = false;
    bool pCoverDownloaded = false;
    bool pTagEdited = false;
} flags_t;

typedef struct args{
    std::string albumName;
    std::string artistName;
    std::string url;
    std::string fullPath;
} data_t;

bool parseArguments(int argc, char **argv, flags_t* flag_struct, data_t* data);
bool downloadAudio(data_t data, flags_t* flags);
bool searchMetadata(data_t data, flags_t* flags, std::vector<std::string>* titles, std::vector<MetadataSearcher::MP3Tag>** metadata);
void editTagsAndCover(data_t data, flags_t* flags,  std::vector<std::string> titles, std::vector<MetadataSearcher::MP3Tag>* metadata);
void removeTempFiles(std::vector<MetadataSearcher::MP3Tag>* metadata);
void endProgram(flags_t flags);

#endif