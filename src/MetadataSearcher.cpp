#include "MetadataSearcher.hpp"
#include "TagEditor.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <musicbrainz5/MediumList.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/Track.h>
#include <musicbrainz5/Metadata.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/RelationList.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/ReleaseList.h>
#include <musicbrainz5/TrackList.h>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

MetadataSearcher::MetadataSearcher(){}
MetadataSearcher::~MetadataSearcher(){}

vector<TagEditor::MP3Tag>* MetadataSearcher::searchAlbum(string album, string artist, int year)
{
    CQuery query("trax");
    CQuery trackQuery("trax");
    setParams(album, artist, year);
    setLookupParams();
    vector<TagEditor::MP3Tag>* result = new vector<TagEditor::MP3Tag>();

    CMetadata metadata = query.Query("release", "", "", params);    
    CReleaseList* albumList = metadata.ReleaseList();
    if(!albumList || albumList->Count() == 0)
    {
        cout << "No albums found that parameters" << endl;
        return NULL;
    }

    string firstAlbumId = albumList->Item(0)->ID();
    CMetadata fullRelease = trackQuery.Query("release",firstAlbumId, "", lookupParams);
    CRelease* firstAlbum = fullRelease.Release();

    if (!firstAlbum || !firstAlbum->MediumList() || firstAlbum->MediumList()->Count() == 0)
    {
            cout << "No medium found" << endl;
            return NULL;
    }
    CMedium* medium = firstAlbum->MediumList()->Item(0);
    CTrackList* tracks = medium->TrackList();

    for(int i = 0; i < tracks->Count();i++)        
    {
        CTrack* singleTrack = tracks->Item(i);
        if(!singleTrack) continue;

        TagEditor::MP3Tag track;
        track.Title = singleTrack->Recording()->Title();
        track.Album = firstAlbum->Title();
        track.Year = firstAlbum->Date();
        track.Artist = "";
        for(int j = 0; j < firstAlbum->ArtistCredit()->NameCreditList()->Count(); j++)
        {

            track.Artist += firstAlbum->ArtistCredit()->NameCreditList()->Item(j)->Name();
            if(j != firstAlbum->ArtistCredit()->NameCreditList()->Count() - 1)
                track.Album += ", ";
        }
        result->push_back(track);
    }
    return result;
    
}


TagEditor::MP3Tag* MetadataSearcher::searchSong(string songName, string album, string artist, int year)
{
    CQuery query("trax");
    setParams(songName, album, artist, year);
    return nullptr;
}


void MetadataSearcher::setParams(string album, string artist, int year = 0)
{
    if(year == 0)
        params["query"] = "release:\""+ album +"\" AND artist:\""+ artist +"\"";
    else
        params["query"] = "release:\""+ album +"\" AND artist:\""+ artist +"\"" + " AND date:" + to_string(year);
}


void MetadataSearcher::setParams(string songName, string album, string artist, int year = 0)
{
    if(year == 0)
        params["query"] = "recording:\"" + songName + "\" release:\""+ album +"\" AND artist:\""+ artist +"\"";
    else
        params["query"] = "recording:\"" + songName + "\" AND release:\""+ album +"\" AND artist:\""+ artist +"\"" + " AND date:" + to_string(year);
}



void MetadataSearcher::setLookupParams()
{
    lookupParams["inc"] = "recordings artist-credits";
}



void MetadataSearcher::resetParams()
{
    params["query"] = "";
    lookupParams["query"] = "";
}