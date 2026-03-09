#include "utils.hpp"
#include <filesystem>

using namespace std;

void retrieveSongsNames(string dirPath, vector<string>*songsNamesVector)
{
    for(auto &entry : filesystem::directory_iterator(dirPath))
    {
        string singleFilePath = entry.path().string();
        int pos = singleFilePath.find(dirPath);
        songsNamesVector->push_back(singleFilePath.substr(pos + dirPath.size()));
    }
}
