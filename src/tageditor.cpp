#include "tageditor.hpp"
#include "MetadataSearcher.hpp"
#include <exception>
#include <taglib/textidentificationframe.h>
#include "taglib/tag.h"
#include "taglib/fileref.h"
#include "taglib/id3v2tag.h"
#include "taglib/attachedpictureframe.h"
#include "taglib/mpegfile.h"
#include <fstream>
#include <iostream>
#include <string>
#include <taglib/tstring.h>
#include "utils.hpp"

using namespace std;

void editTags(vector<string> songNames, string songsDirPath, vector<MetadataSearcher::MP3Tag>* metadatas, string artistName, bool noImage, bool complexName)
{
    for(int i = 0; i < songNames.size(); i++)
    {
        TagLib::MPEG::File file((songsDirPath+songNames[i]).c_str());
        if(!file.isValid())
            continue;
        MetadataSearcher::MP3Tag tag;
        bool songFound = false;
        for(MetadataSearcher::MP3Tag t : *metadatas)
        {
            string fileNameStr = songNames[i];
            string songNameStr = t.Title;
            prepareStringForComparison(&fileNameStr);
            prepareStringForComparison(&songNameStr);

            if(fileNameStr.find(songNameStr) != string::npos)
            {
                // found!
                songFound = true;
                tag = t;
                break;
            }
            else 
                continue;
        }
        if (!songFound)
            continue;

        // track info
        file.tag()->setTitle(TagLib::String((tag.Title.empty() || tag.Title == " ") ? string("song_" + to_string(i)) : tag.Title, TagLib::String::UTF8));
        file.tag()->setAlbum(TagLib::String(tag.Album, TagLib::String::UTF8));
        file.tag()->setArtist(tag.Artist.empty() ? TagLib::String(artistName, TagLib::String::UTF8) : tag.Artist);

        try{
            file.tag()->setTrack(stoi(tag.TrackNumber));
        }
        catch (exception e){
            file.tag()->setTrack(i+1);
        }
        try{
            file.tag()->setYear(stoi((tag.Year).substr(0, 4)));
        } 
        catch (exception e){
            file.tag()->setYear(0);
        }

        file.tag()->setGenre(TagLib::String(tag.Genre, TagLib::String::UTF8));

        try{
            // album artist 
            TagLib::ID3v2::Tag* tagV2 = file.ID3v2Tag(true);
            tagV2->removeFrames("TPE2");

            auto* identificationFrame = new TagLib::ID3v2::TextIdentificationFrame("TPE2", TagLib::String::UTF16);
            identificationFrame->setText(TagLib::String(artistName, TagLib::String::UTF8));
            tagV2->addFrame(identificationFrame);
        }
        catch(exception e){
            cout << "[WARNING] Album artist error! Field skipped." << endl;
        }

        // edit image
        if(!noImage){
            try{
                // image
                ifstream imageFile(string("./" + tag.AlbumID + "-front.jpg"), ios::binary);
                vector<char> imageData{istreambuf_iterator<char>(imageFile), istreambuf_iterator<char>{}};
                TagLib::ID3v2::Tag* tagV2 = file.ID3v2Tag(true);
                tagV2->removeFrames("APIC");
                
                auto* frame = new TagLib::ID3v2::AttachedPictureFrame();
                frame->setMimeType("image/jpeg");  // or "image/png"
                frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
                frame->setDescription("Front Cover");
                frame->setPicture(TagLib::ByteVector(imageData.data(), imageData.size()));
                tagV2->addFrame(frame);
            }
            catch(exception e )
            {
                cout << "[WARNING] Cover art attachment skipped." << endl;
            }
        }
        
        file.save();

        // rename file
        std::string ext = songNames[i].substr(songNames[i].find_last_of("."));
        rename((songsDirPath + songNames[i]).c_str(), (songsDirPath + (complexName ? ((tag.TrackNumber.size() == 2 ? tag.TrackNumber : "0" + tag.TrackNumber) + " - " + artistName + " - " + tag.Title) : tag.Title) + ext).c_str());
    }
}



void editTag(std::string songName, MetadataSearcher::MP3Tag* metadata, std::string artistName, bool noImage, bool complexName){
    std::string currentPath = "./";
    TagLib::MPEG::File file((currentPath+songName).c_str());
        if(!file.isValid())
            return;
        file.tag()->setTitle(TagLib::String((metadata->Title.empty() || metadata->Title == " ") ? string("song") : metadata->Title, TagLib::String::UTF8));
        file.tag()->setAlbum(TagLib::String(metadata->Album, TagLib::String::UTF8));
        file.tag()->setArtist(metadata->Artist.empty() ? TagLib::String(artistName, TagLib::String::UTF8) : metadata->Artist);
        try {
            file.tag()->setTrack(stoi(metadata->TrackNumber));
        }
        catch (exception e) {
            file.tag()->setTrack(1);
        }
        try {
            file.tag()->setYear(stoi((metadata->Year).substr(0, 4)));
        } 
        catch (exception e) {
            file.tag()->setYear(0);
        }

        file.tag()->setGenre(TagLib::String(metadata->Genre, TagLib::String::UTF8));

        try{
            // album artist 
            TagLib::ID3v2::Tag* tagV2 = file.ID3v2Tag(true);
            tagV2->removeFrames("TPE2");

            auto* identificationFrame = new TagLib::ID3v2::TextIdentificationFrame("TPE2", TagLib::String::UTF16);
            identificationFrame->setText(TagLib::String(artistName, TagLib::String::UTF8));
            tagV2->addFrame(identificationFrame);
        }
        catch(exception e){
            cout << "[WARNING] Album artist error! Field skipped." << endl;
        }
        
        // edit image
        if(!noImage){
            try{
                ifstream imageFile(string("./" + metadata->AlbumID + "-front.jpg"), ios::binary);
                vector<char> imageData{istreambuf_iterator<char>(imageFile), istreambuf_iterator<char>{}};
                TagLib::ID3v2::Tag* tagV2 = file.ID3v2Tag(true);
                tagV2->removeFrames("APIC");
        
                // Create the picture frame
                auto* frame = new TagLib::ID3v2::AttachedPictureFrame();
                frame->setMimeType("image/jpeg");  // or "image/png"
                frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
                frame->setDescription("Front Cover");
                frame->setPicture(TagLib::ByteVector(imageData.data(), imageData.size()));
        
                // Attach and save
                tagV2->addFrame(frame);
            }
            catch(exception e ){
                cout << "[WARNING] Cover art attachment skipped." << endl;
            }
        }
        
        file.save();

        // rename file
        std::string ext = songName.substr(songName.find_last_of("."));
        rename((currentPath + songName).c_str(), (currentPath + (complexName ? ((metadata->TrackNumber.size() == 2 ? metadata->TrackNumber : "0" + metadata->TrackNumber) + " - " + artistName + " - " + metadata->Title) : metadata->Title) + ext).c_str());
}
