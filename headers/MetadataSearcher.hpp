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
/*

    CQuery q("trax");
    try{
        CQuery::tParamMap params;
        params["query"] = "release:\"Nightmare\" AND artist:\"Avenged Sevenfold\"";
        CMetadata meta = q.Query("release", "", "", params);
        auto* list = meta.ReleaseList();
        if(!list || list->Count() == 0)
        {
            std::cout << "Error" << std::endl;
            return -1;
        }

        for(int i = 0; i <list->Count(); i++)
        {
            auto* record = list->Item(i);
            std::cout << record->Title() << std::endl;
            std::cout << record->Date() << std::endl;
            std::cout << record->ArtistCredit()->NameCreditList()->Item(0)->Artist()->Name() << std::endl;
        }

    }catch(std::exception e){
        std::cout << "Exception " << std::endl;
        return 0;
    }

*/