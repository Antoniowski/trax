#include <string>
#include <vector>
#include "MetadataSearcher.hpp"



/**
 * @brief 
 * 
 * @param songNames vector of files names
 * @param songsDirPath path of the dir containing audio files
 * @param metadatas MP3Tag vector pointer that contains the found tags that will be used to edit the files' ones
 * @param artistName artist name string
 */
void editTags(std::vector<std::string> songNames, std::string songsDirPath,  std::vector<MetadataSearcher::MP3Tag>* metadatas, std::string artistName);