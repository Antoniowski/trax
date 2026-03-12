#ifndef YT_DLP_H
#define YT_DLP_H
#include <string>

int downloadPlaylist(std::string url,std::string album, std::string artist, bool debug = false);
int downloadSong(std::string url, bool debug = false);

#endif