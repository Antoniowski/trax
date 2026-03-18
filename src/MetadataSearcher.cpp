#include "MetadataSearcher.hpp"
#include <cstddef>
#include "coverart/CoverArt.h"
#include <fstream>
#include <iostream>
#include <musicbrainz5/MediumList.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/TagList.h>
#include <musicbrainz5/Track.h>
#include <musicbrainz5/Metadata.h>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/RelationList.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/ReleaseList.h>
#include <musicbrainz5/TrackList.h>
#include <musicbrainz5/TagList.h>
#include <musicbrainz5/Tag.h>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

MetadataSearcher::MetadataSearcher(){}
MetadataSearcher::~MetadataSearcher(){}

vector<MetadataSearcher::MP3Tag>* MetadataSearcher::searchAlbum(string album, string artist, int year, int iteration)
{
    CQuery query("trax");
    CQuery trackQuery("trax");
    setParams(album, artist, year);
    setLookupParams();
    vector<MetadataSearcher::MP3Tag>* result = new vector<MetadataSearcher::MP3Tag>();
    CMetadata metadata = query.Query("release", "", "", params);    
    CReleaseList* albumList = metadata.ReleaseList();
    if(!albumList || albumList->Count() == 0)
    {
        cout << "No albums found that parameters" << endl;
        delete result;
        return NULL;
    }
    
    string firstAlbumId;
    if(iteration != 0 && albumList->Count() - 1 >= iteration){
        firstAlbumId = albumList->Item(iteration)->ID();
    }else{
        firstAlbumId = albumList->Item(0)->ID();
    }

    CMetadata fullRelease = trackQuery.Query("release",firstAlbumId, "", lookupParams);
    CRelease* firstAlbum = fullRelease.Release();

    if (!firstAlbum || !firstAlbum->MediumList() || firstAlbum->MediumList()->Count() == 0)
    {
            cout << "No medium found" << endl;
            delete result;
            return NULL;
    }

    CTagList* genresList = fullRelease.TagList();
    string genresString = "";
    if (genresList) {
        for(int i = 0; i < genresList->NumItems(); i++){
            if(!genresString.empty()) genresString += "/";
            genresString += genresList->Item(i)->Name();
        }
    }

    // Songs
    int currentMedium = 0;
    while (currentMedium < firstAlbum->MediumList()->Count()) {
        CMedium* medium = firstAlbum->MediumList()->Item(currentMedium);
        CTrackList* tracks = medium->TrackList();
        CTrack* singleTrack = NULL;
        int count = tracks->Count();
    
        for(int i = 0; i < count; i++)        
        {
            singleTrack = tracks->Item(i);
            if(!singleTrack) continue;
    
            MP3Tag track;
            track.AlbumID = firstAlbumId;
            track.Title = singleTrack->Recording()->Title();
            if (track.Title.empty()) singleTrack->Title();
            track.Album = firstAlbum->Title();
            track.Year = firstAlbum->Date();
            track.TrackNumber = singleTrack->Number();
            track.Artist = readArtists(singleTrack->Recording()->ArtistCredit());
            if(track.Artist.empty()) track.Artist = readArtists(singleTrack->ArtistCredit());
            if(track.Artist.empty()) track.Artist = readArtists(firstAlbum->ArtistCredit());
            track.Genre = genresString;
            result->push_back(track);
        }
        currentMedium++;
    }

    resetParams();
    return result;
}


MetadataSearcher::MP3Tag* MetadataSearcher::searchSong(string songName, string album, string artist, int year)
{
    CQuery query("trax");
    CQuery trackQuery("trax");
    setParams(album, artist, year);
    setLookupParams();
    MP3Tag* result = new MP3Tag();

    CMetadata metadata = query.Query("release", "", "", params);    
    CReleaseList* albumList = metadata.ReleaseList();
    if(!albumList || albumList->Count() == 0)
    {
        cout << "No albums found that parameters" << endl;
        delete result;
        return NULL;
    }

    string firstAlbumId = albumList->Item(0)->ID();
    CMetadata fullRelease = trackQuery.Query("release",firstAlbumId, "", lookupParams);
    CRelease* firstAlbum = fullRelease.Release();

    if (!firstAlbum || !firstAlbum->MediumList() || firstAlbum->MediumList()->Count() == 0)
    {
            cout << "No medium found" << endl;
            delete result;
            return NULL;
    }
    CMedium* medium = firstAlbum->MediumList()->Item(0);
    CTrackList* tracks = medium->TrackList();
    CTrack* singleTrack = NULL;

    for(int i = 0; i < tracks->Count();i++)        
    {
        singleTrack = tracks->Item(i);
        if(!singleTrack) continue;
        if(!(singleTrack->Recording()->Title().find(songName) != string::npos)) continue;

        MetadataSearcher::MP3Tag track;
        track.Title = singleTrack->Recording()->Title();
        track.Album = firstAlbum->Title();
        track.Year = firstAlbum->Date();
        track.Artist = readArtists(singleTrack->Recording()->ArtistCredit());
        if(track.Artist.empty()) track.Artist = readArtists(singleTrack->ArtistCredit());
        if(track.Artist.empty()) track.Artist = readArtists(firstAlbum->ArtistCredit());
        track.TrackNumber = singleTrack->Number();
        *result = track;
        return result;
    }
    delete result;
    resetParams();
    return NULL;
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
    lookupParams["inc"] = "recordings artist-credits genres";
}



void MetadataSearcher::resetParams()
{
    params["query"] = "";
    lookupParams["query"] = "";
}


string MetadataSearcher::readArtists(CArtistCredit* ac){
    if (!ac || !ac->NameCreditList()) return "";
    string result;
    CNameCreditList* ncl = ac->NameCreditList();
    for (int j = 0; j < ncl->Count(); j++) {
        CNameCredit* nc = ncl->Item(j);
        if (!nc || !nc->Artist()) continue;
        if (!result.empty()) result += ", ";
        result += nc->Artist()->Name();
    }
    return result;
};

void MetadataSearcher::downloadCoverArt(string albumID)
{
    //save cover art
    CoverArtArchive::CCoverArt coverArt("trax");
    vector<unsigned char> images = coverArt.FetchFront(albumID);
    if(images.size())
    {
        stringstream filename;
        filename << albumID << "-front.jpg";
        ofstream front(filename.str().c_str());
        front.write((const char*)&images[0], images.size());
        front.close();
    }
}