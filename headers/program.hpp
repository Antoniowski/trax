#ifndef _PROGRAM_H
#define _PROGRAM_H
#include <string>

typedef struct flags{
    bool menu = false;
    bool debug = false;
    bool singleMode = false;
    bool noMetadataMode = false;
    bool downloaded = false;
} flags_t;

typedef struct args{
    std::string albumName;
    std::string artistName;
    std::string url;
    std::string fullPath;
} data_t;

bool parseArguments(int argc, char **argv, flags_t* flag_struct, data_t* data);

#endif