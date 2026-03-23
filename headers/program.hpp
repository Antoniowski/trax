#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "MetadataSearcher.hpp"
#include "spinner.hpp"
#include <string>
#include <vector>

typedef struct flags{
    bool printMenu = false;
    bool printVersion = false;
    bool debug = false;
    bool singleMode = false;
    bool skipMetadata = false;
    bool skipDownload = false;
    bool keepImage = false;
    bool skipImageDownload = false;
    bool complexName = false;
    bool addGenres = false;
    std::string format = "mp3";
    int iteration = 0;
    bool pAudioDownloaded = false;
    bool pCoverDownloaded = false;
    bool pTagEdited = false;
    bool pError = false;
} flags_t;

typedef struct args{
    std::string albumName = "";
    std::string artistName = "";
    std::string songName = "";
    int year = 0;
    std::string url = "";
    std::string fullPath = "";
    std::string genres = "";
} data_t;

enum Phases{
    STARTING,
    DOWNLOAD,
    METADATA_AND_COVER,
    TAG_EDIT,
};

/**
 * @brief function to parse main args and fill flags_t and data_t data structures
 * 
 * @param argc number of arguments
 * @param argv arguments string pointer
 * @param flag_struct pointer to structure containing all program flags
 * @param data pointer to structure containing program useful data
 * @return true if parsing is completed successfully
 * @return false if parsing fails
 */
bool parseArguments(int argc, char **argv, flags_t* flag_struct, data_t* data);


/**
 * @brief function that handles the audio download. Based on what is contained in the flags struct it will start
 * the playlist download or the single mode download. 
 * 
 * @param data pointer to structure containing program useful data
 * @param flags pointer to structure containing all program flags
 * @return true if download is completed successfully
 * @return false if download fails
 */
bool downloadAudio(data_t data, flags_t* flags);


/**
 * @brief function that handles the metadata research using a query to the MusicBrainz5 database. The result is saved using the metadata parameter pointer. 
 * This also create a vector with all the audio files names that is saved using the titles parameter pointer
 * 
 * @param data pointer to structure containing program useful data
 * @param flags pointer to structure containing all program flags
 * @param titles pointer to the vector that will contains the resulting vector with files names
 * @param metadata pointer to the pointer that will be used to save the result of the query
 * @return true if the research is finished without problems
 * @return false if the research fails 
 */
bool searchMetadata(data_t data, flags_t* flags, std::vector<std::string>* titles, std::vector<MetadataSearcher::MP3Tag>** metadata);


/**
 * @brief function that handles the metadata research using a query to the MusicBrainz5 database. The result is saved using the metadata parameter pointer. 
 * 
 * @param data pointer to structure containing program useful data
 * @param flags pointer to structure containing all program flags
 * @param songFileName name of the downloaded audio file
 * @param metadata pointer to the pointer that will be used to save the result of the query
 * @return true if the research is finished without problems
 * @return false if the research fails 
 */
bool searchMetadata(data_t data, flags_t* flags, std::string* songFileName, MetadataSearcher::MP3Tag** metadata);


/**
 * @brief function that handles the tag editing phase of the program. This will look for the folder containing the downloaded audios and will iterate them.
 * If the file name corresponds to or contains one of the songs title stored in the metadata vector, then the function will edit its tag using the information
 * of the correct retrieved tag and then will also rename the file with the song title. As last thing, this function is also responsible of the downlaod and 
 * attachment of the cover art of the album
 * 
 * @param data structure containing program useful data
 * @param flags pointer to structure containing all program flags
 * @param titles vector that contains the audio files names
 * @param metadata pointer to the pointer contaings the tags of each song of the searched album
 */
void editTagsAndCover(data_t data, flags_t* flags,  std::vector<std::string> titles, std::vector<MetadataSearcher::MP3Tag>* metadata);


/**
 * @brief function that handles the tag editing phase of the program. This is will look for the audio file containing the song name and then will edit
 * its tag and cover
 * @param data structure containing program useful data
 * @param flags pointer to structure containing all program flags
 * @param songFileName name of the audio
 * @param metadata pointer to the MP#Tag that will  contain the file tag
 */
void editTagsAndCover(data_t data, flags_t* flags, std::string songFileName, MetadataSearcher::MP3Tag *metadata);


/**
 * @brief remove the temporary files created during the metadata research
 * 
 * @param metadata pointer to the pointer contaings the tags of each song of the searched album. It is used to retrieve the downloaded album ID.
 */
void removeTempFiles(std::vector<MetadataSearcher::MP3Tag>* metadata);


/**
 * @brief remove the temporary files created during the metadata research
 * 
 * @param metadata pointer to the MP3Tag contaings the info of the song.
 */
void removeTempFiles(MetadataSearcher::MP3Tag* metadata);



/**
 * @brief end the program by printing the final status of each operation
 * 
 * @param flags pointer to structure containing all program flags
 */
void endProgram(flags_t flags);


/**
 * @brief setup spinner text and type based on current program phase
 * 
 * @param spinner reference to the spinner that will be modified
 * @param currentPhase current program phase
 */
void setupSpinner(spinners::Spinner** spinner, Phases currentPhase);


/**
 * @brief Print current program version
 * 
 */
void printVersion();

#endif