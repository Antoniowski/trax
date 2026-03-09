#include "yt_dlp.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include "spinner.hpp"

int downloadPlaylist(std::string url, std::string album, std::string artist, bool debug = false){
    std::string cmd = "yt-dlp -t mp3 --yes-playlist  -P \"./" + artist + " - " + album + "\" \"" + url + "\"";
    
    // if in not in debug mode i want to ignore the yt-dlp output stream
    if(!debug)
        cmd += " > /dev/null 2>&1";
    
    FILE* pipe = popen(cmd.c_str(), "r");
    if(!pipe){
        std::cout << "Error in opening pipe" << std::endl;
        return -1;
    }

    if(debug){
        // drain the pipe buffer before closing
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::cout << buffer; 
        }
        
        int result = pclose(pipe);
        return WEXITSTATUS(result); 
    }else{
        spinners::Spinner* spinner = new spinners::Spinner();
        spinner->setText("Downlading ...");
        spinner->setInterval(100);
        spinner->setSymbols("dots4");

        spinner->start();

        int result = pclose(pipe);
        spinner->stop();
        
        return WEXITSTATUS(result); 
    }
}


int downloadSong(std::string url, std::string album, std::string artist, bool debug = false){
    std::string cmd = "yt-dlp -t mp3 -P \"./" + artist + " - " + album + "\" \"" + url + "\"";

    // if in not in debug mode i want to ignore the yt-dlp output stream
    if(!debug)
        cmd += " > /dev/null 2>&1";

    FILE* pipe = popen(cmd.c_str(), "r");
    if(!pipe){
        std::cout << "[ERROR] Error in opening pipe" << std::endl;
        return -1;
    }

    if(debug){
        // drain the pipe buffer before closing
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::cout << buffer; 
        }
        
        int result = pclose(pipe);
        return WEXITSTATUS(result); 
    }else{
        spinners::Spinner* spinner = new spinners::Spinner();
        spinner->setText("Downlading ...");
        spinner->setInterval(100);
        spinner->setSymbols("dots4");

        spinner->start();

        int result = pclose(pipe);
        spinner->stop();
        
        return WEXITSTATUS(result); 
    }
}