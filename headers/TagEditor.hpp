/**
 * @file TagEditor.hpp
 * @author Antonio 'Antoniowski' Romano 
 * @brief Mp3 tag editor based on the miker525's repo (https://gist.github.com/miker525/73778238350b0412e115) 
 * @version 0.1
 * @date 2026-02-22
 *
 */

#ifndef TAG_EDITOR_H
#define TAG_EDITOR_H

#include <string>

class TagEditor{
    public:
        struct MP3Tag{
            std::string Artist;
            std::string Title;
            std::string Album;
            std::string Year;
            std::string Comment;
        };

        TagEditor();
        ~TagEditor();

        int getsize(FILE *file);
        bool checkmp3(std::string filename);
        std::string ReadThirty(FILE *file, int readloc);
        std::string Header(FILE *file, int readloc);
        std::string Year(FILE *file, int readloc);
        void ReadMP3(FILE *file, MP3Tag tag);
        void WriteArtist(FILE *file, int filesize, char Artist[]);
        void WriteTitle(FILE *file, int filesize, char Title[]);
        void WriteAlbum(FILE *file, int filesize, char Album[]);
        void WriteYear(FILE *file, int filesize, char Year[]);
        void WriteComment(FILE *file, int filesize, char Comment[]);
};

#endif