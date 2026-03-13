#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <vector>


/**
 * @brief get name of the files (songs) that are contained in the dirPath directory
 * 
 * @param dirPath path of the dir containing files
 * @param songsNamesVector reference of the vector that will contains the result
 */
void retrieveSongsNames(std::string dirPath, std::vector<std::string>*songsNamesVector);


/**
 * @brief prepare a string to be compared. The string will be trimmed, lowercased and all the special chars will be removed
 * 
 * @param string reference of the string that will be manipulated
 */
void prepareStringForComparison(std::string* string);

#endif