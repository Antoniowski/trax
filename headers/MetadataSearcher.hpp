#ifndef METADATA_SEARCHER_H
#define METADATA_SEARCHER_H

#include <array>
#include <musicbrainz5/Metadata.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/Artist.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/NameCreditList.h>
#include <string>
#include <vector>


using namespace MusicBrainz5;

// query keys
// recording:song title
// artist:artist name
// release:album name
// date:release year
// dur:duration in ms

class MetadataSearcher{
    public:
        struct MP3Tag
        {
            std::string AlbumID;
            std::string Artist;
            std::string Title;
            std::string Album;
            std::string Year;
            std::string Comment;
            std::string TrackNumber;
        };  
        
        MetadataSearcher();
        ~MetadataSearcher();
        std::vector<MP3Tag>* searchAlbum(std::string album, std::string artist, int year = 0);
        MP3Tag* searchSong(std::string songName, std::string album, std::string artist, int year = 0);
        void downloadCoverArt(std::string albumID);

    private:
        CQuery::tParamMap params;
        CQuery::tParamMap lookupParams;

        void setParams(std::string album, std::string artist, int year);
        void setParams(std::string songName, std::string album, std::string artist, int year);
        void setLookupParams();
        void resetParams();
        std::string readArtists(CArtistCredit* ac);
};
#endif