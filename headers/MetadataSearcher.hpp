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