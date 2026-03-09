#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <vector>

void retrieveSongsNames(std::string dirPath, std::vector<std::string>*songsNamesVector);
void prepareStringForComparison(std::string* string);

#endif