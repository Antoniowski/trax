#include <string>
#include <vector>
#include "MetadataSearcher.hpp"



/**
 * @brief edit tags of the files contained in the directory at the songdDirPath
 * 
 * @param songNames vector of files names
 * @param songsDirPath path of the dir containing audio files
 * @param metadatas MP3Tag vector pointer that contains the found tags that will be used to edit the files' ones
 * @param artistName artist name string
 */
void editTags(std::vector<std::string> songNames, std::string songsDirPath,  std::vector<MetadataSearcher::MP3Tag>* metadatas, std::string artistName, std::string genres, bool rawGenres, bool noImage = false, bool complexName = false);


/**
 * @brief edit tags of the file contained in the current directory containig the song name
 * 
 * @param songName audio file name
 * @param metadatas MP3Tag pointer that contains the found tag that will be used to edit the file's tag
 * @param artistName artist name string
 */
void editTag(std::string songName, MetadataSearcher::MP3Tag* metadatas, std::string artistName, std::string genres, bool rawGenres, bool noImage, bool complexName = false);
