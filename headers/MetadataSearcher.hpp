#ifndef METADATA_SEARCHER_H
#define METADATA_SEARCHER_H

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
            std::string ArtistSortName;
            std::string Title;
            std::string Album;
            std::string Year;
            std::string TrackNumber;
        };  
        
        MetadataSearcher();
        ~MetadataSearcher();


        /**
         * @brief handles the query to the MusicBrainz5 database to retrieve tags of all the songs of a specific album
         * 
         * @param album album name (case sensitive)
         * @param artist artist name (case sensitive)
         * @param year year when the album was released
         * @param iteration parameter used to specify the item to retrive from the album list result
         * @return std::vector<MP3Tag>* vector with all the tags of the songs of the first album retrieved
         */
        std::vector<MP3Tag>* searchAlbum(std::string album, std::string artist, int year = 0, int iteration = 0);


        /**
         * @brief handles the query to the MusicBrainz5 database to retrieve the tag of a specific song of a specific album
         * 
         * @param songName song name (case sensitive)
         * @param album album name (case sensitive)
         * @param artist artist name (case sensitive)
         * @param year year when the album was released
         * @param iteration parameter used to specify the item to retrive from the album list result
         * @return MP3Tag* tag of the specified song of the first album retrieved
         */
        MP3Tag* searchSong(std::string songName, std::string album, std::string artist, int year = 0, int iteration = 0);


        /**
         * @brief handles the download of the cover art of a specified album
         * 
         * @param albumID id of the album of which the program will download the cover art
         */
        void downloadCoverArt(std::string albumID);

    private:
        CQuery::tParamMap params;
        CQuery::tParamMap lookupParams;


        /**
         * @brief Set the Params object
         * 
         * @param album album name (case sensitive)
         * @param artist artist name (case sensitive)
         * @param year year when the album was released
         */
        void setParams(std::string album, std::string artist, int year = 0);


        /**
         * @brief Set the Params object
         * 
         * @param songName song name (case sensitive)
         * @param album album name (case sensitive)
         * @param artist artist name (case sensitive)
         * @param year year when the album was released
         */
        void setParams(std::string songName, std::string album, std::string artist, int year = 0);


        /**
         * @brief Set the Lookup Params object
         * 
         */
        void setLookupParams();


        /**
         * @brief reset all the params objects
         * 
         */
        void resetParams();


        /**
         * @brief retrieve a single string containing all the artist of the specified CArtistCredit object 
         * 
         * @param ac pointer containing the CArtistCredit that will be used to retrive the artist string
         * @return std::string containing all the artist found in the CArtistCredit object
         */
        std::string readArtists(CArtistCredit* ac);

/**
         * @brief retrieve a single string containing all the artist sort name of the specified CArtistCredit object 
         * 
         * @param ac pointer containing the CArtistCredit that will be used to retrive the artist string
         * @return std::string containing all the artist found in the CArtistCredit object
         */
        std::string readArtistsSortName(CArtistCredit* ac);

};
#endif