#ifndef YT_DLP_H
#define YT_DLP_H
#include <string>

/**
 * @brief handles the download the playlist linked to the specified url using yt-dlp. Album and artist parameter
 * will be used to create a dir where all the downloaded files will be saved 
 * 
 * @param url url of the playlist to download
 * @param album album name
 * @param artist artist name
 * @param fileFormat file extension used to save audio
 * @param debug flag that indicates if the program was launched in debug mode
 * @return int 
 */
int downloadPlaylist(std::string url,std::string album, std::string artist, std::string fileFormat = "mp3", bool debug = false);


/**
 * @brief handle the download of the audio linked to the url. The audio will be saved in the 
 * dir where the program was launched.
 * 
 * @param url url of the song that will be downloaded
 * @param fileFormat file extension used to save audio
 * @param debug flag that indicates if the program was launched in debug mode
 * @return int 
 */
int downloadSong(std::string url, std::string audioFormat = "mp3", bool debug = false);

#endif