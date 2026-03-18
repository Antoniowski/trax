#include "utils.hpp"
#include <filesystem>
#include <string>
#include <algorithm>

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


void retrieveSingleFileSongName(std::string songNameToFind, std::string* fileName){
    for(auto &entry : filesystem::directory_iterator("./"))
    {   
        string currentPath = "./";
        string singleFilePath = entry.path().string();
        string str = songNameToFind;
        string currentFileLookedName = "";
        string currentFileLookedNamePrepared = "";
        
        int pos = singleFilePath.find(currentPath);
        currentFileLookedName = singleFilePath.substr(pos + currentPath.size());
        currentFileLookedNamePrepared = currentFileLookedName;
        
        prepareStringForComparison(&str);
        prepareStringForComparison(&currentFileLookedNamePrepared);

        if(currentFileLookedNamePrepared.find(str) != string::npos){
            //found file
            *fileName = currentFileLookedName;
            break;
        }
    }
}


void prepareStringForComparison(std::string* str)
{
    //lowercase + trim + normalize + remove spaces
    transform(str->begin(), str->end(), str->begin(), ::tolower);
    str->erase(remove_if(str->begin(), str->end(), [](char c) {
        return !isalnum(c);
        }), str->end());
    str->erase(0, str->find_first_not_of(" \t\r\n"));
    str->erase(str->find_last_not_of(" \t\r\n") + 1);
}