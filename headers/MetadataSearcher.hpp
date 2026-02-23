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


using namespace MusicBrainz5;

class MetadataSearcher{
    public:
        MetadataSearcher();
        ~MetadataSearcher();
        void searchAlbum();
        void searchSong();
        void searchArtist();

    private:
        CQuery::tParamMap params;

        void setParams();
        void resetParams();
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