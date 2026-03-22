#include "tageditor.hpp"
#include "MetadataSearcher.hpp"
#include <exception>
#include <taglib/textidentificationframe.h>
#include "taglib/id3v2tag.h"
#include "taglib/attachedpictureframe.h"
#include "taglib/mpegfile.h"
#include <fstream>
#include <iostream>
#include <string>
#include <taglib/tfile.h>
#include <taglib/tstring.h>
#include "utils.hpp"

using namespace std;

void editTags(vector<string> songNames, string songsDirPath, vector<MetadataSearcher::MP3Tag>* metadatas, string artistName, std::string genres, bool noImage, bool complexName){
    for(int i = 0; i < songNames.size(); i++){
        TagLib::MPEG::File file((songsDirPath+songNames[i]).c_str());
        if(!file.isValid()) continue;
        
        TagLib::ID3v2::Tag* tagV2 = file.ID3v2Tag(true);
        if(!tagV2) return;

        MetadataSearcher::MP3Tag tag;
        bool songFound = false;
        for(MetadataSearcher::MP3Tag t : *metadatas){
            string fileNameStr = songNames[i];
            string songNameStr = t.Title;
            prepareStringForComparison(&fileNameStr);
            prepareStringForComparison(&songNameStr);

            if(fileNameStr.find(songNameStr) != string::npos){
                // found!
                songFound = true;
                tag = t;
                break;
            }
            else 
                continue;
        }
        
        if (!songFound) continue;

        // track info
        tagV2->setTitle(TagLib::String((tag.Title.empty() || tag.Title == " ") ? string("song_" + to_string(i)) : tag.Title, TagLib::String::UTF8));
        tagV2->setAlbum(TagLib::String(tag.Album, TagLib::String::UTF8));
        tagV2->setArtist(tag.Artist.empty() ? TagLib::String(artistName, TagLib::String::UTF8) : TagLib::String(tag.Artist, TagLib::String::UTF8));

        try{
            tagV2->setTrack(stoi(tag.TrackNumber));
        }
        catch (exception e){
            tagV2->setTrack(i+1);
        }
        try{
            tagV2->setYear(stoi((tag.Year).substr(0, 4)));
        } 
        catch (exception e){
            tagV2->setYear(0);
        }

        
        tagV2->setGenre(TagLib::String(genres, TagLib::String::UTF8));
        try{
            // album artist 
            tagV2->removeFrames("TPE2");
            auto* identificationFrame = new TagLib::ID3v2::TextIdentificationFrame("TPE2", TagLib::String::UTF16);
            identificationFrame->setText(TagLib::String(artistName, TagLib::String::UTF8));
            tagV2->addFrame(identificationFrame);
        }
        catch(exception e){
            cout << "[WARNING] Album artist error! Field skipped." << endl;
        }

        // TSOP - Artist sort name
        try{
            tagV2->removeFrames("TSOP");
            auto* tsop = new TagLib::ID3v2::TextIdentificationFrame("TSOP", TagLib::String::UTF16);
            tsop->setText(TagLib::String(tag.ArtistSortName, TagLib::String::UTF8));
            tagV2->addFrame(tsop);
        }
        catch(exception e){
            cout << "[WARNING] TSOP frame skipped" << endl;
        }
        

        // TSO2 - Album artist sort name  
        try{
            tagV2->removeFrames("TSO2");
            auto* tso2 = new TagLib::ID3v2::TextIdentificationFrame("TSO2", TagLib::String::UTF16);
            tso2->setText(TagLib::String(artistName, TagLib::String::UTF8));
            tagV2->addFrame(tso2);
        }
        catch(exception e){
            cout << "[WARNING] TSO2 frame skipped." << endl;
        }

        // edit image
        if(!noImage){
            try{
                // image
                ifstream imageFile(string("./" + tag.AlbumID + "-front.jpg"), ios::binary);
                vector<char> imageData{istreambuf_iterator<char>(imageFile), istreambuf_iterator<char>{}};
                tagV2->removeFrames("APIC");
                
                auto* frame = new TagLib::ID3v2::AttachedPictureFrame();
                frame->setMimeType("image/jpeg");  // or "image/png"
                frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
                frame->setDescription("Front Cover");
                frame->setPicture(TagLib::ByteVector(imageData.data(), imageData.size()));
                tagV2->addFrame(frame);
            }
            catch(exception e ){
                cout << "[WARNING] Cover art attachment skipped." << endl;
            }
        }
        
        file.save();

        // rename file
        std::string ext = songNames[i].substr(songNames[i].find_last_of("."));
        rename((songsDirPath + songNames[i]).c_str(), (songsDirPath + (complexName ? ((tag.TrackNumber.size() == 2 ? tag.TrackNumber : "0" + tag.TrackNumber) + " - " + tag.ArtistSortName + " - " + tag.Title) : tag.Title) + ext).c_str());
    }
}



void editTag(std::string songName, MetadataSearcher::MP3Tag* metadata, std::string artistName, std::string genres, bool noImage, bool complexName){
    std::string currentPath = "./";
    TagLib::MPEG::File file((currentPath+songName).c_str());
    if(!file.isValid()) return;

    TagLib::ID3v2::Tag* tagV2 = file.ID3v2Tag(true);
    if(!tagV2) return;

    tagV2->setTitle(TagLib::String((metadata->Title.empty() || metadata->Title == " ") ? string("song") : metadata->Title, TagLib::String::UTF8));
    tagV2->setAlbum(TagLib::String(metadata->Album, TagLib::String::UTF8));
    tagV2->setArtist(metadata->Artist.empty() ? TagLib::String(artistName, TagLib::String::UTF8) : TagLib::String(metadata->Artist, TagLib::String::UTF8));

    try {
        tagV2->setTrack(stoi(metadata->TrackNumber));
    }
    catch (exception e) {
        tagV2->setTrack(1);
    }
    try {
        tagV2->setYear(stoi((metadata->Year).substr(0, 4)));
    } 
    catch (exception e) {
        tagV2->setYear(0);
    }

    tagV2->setGenre(TagLib::String(genres, TagLib::String::UTF8));

    try{
        // album artist 
        tagV2->removeFrames("TPE2");

        auto* identificationFrame = new TagLib::ID3v2::TextIdentificationFrame("TPE2", TagLib::String::UTF16);
        identificationFrame->setText(TagLib::String(artistName, TagLib::String::UTF8));
        tagV2->addFrame(identificationFrame);
    }
    catch(exception e){
        cout << "[WARNING] Album artist error! Field skipped." << endl;
    }

    // TSOP - Artist sort name
    try{
        tagV2->removeFrames("TSOP");
        auto* tsop = new TagLib::ID3v2::TextIdentificationFrame("TSOP", TagLib::String::UTF16);
        tsop->setText(TagLib::String(metadata->ArtistSortName, TagLib::String::UTF8));
        tagV2->addFrame(tsop);
    }
    catch(exception e){
        cout << "[WARNING] TSOP frame skipped" << endl;
    }
    

    // TSO2 - Album artist sort name  
    try{
        tagV2->removeFrames("TSO2");
        auto* tso2 = new TagLib::ID3v2::TextIdentificationFrame("TSO2", TagLib::String::UTF16);
        tso2->setText(TagLib::String(artistName, TagLib::String::UTF8));
        tagV2->addFrame(tso2);
    }
    catch(exception e){
        cout << "[WARNING] TSO2 frame skipped." << endl;
    }
    
    // edit image
    if(!noImage){
        try{
            ifstream imageFile(string("./" + metadata->AlbumID + "-front.jpg"), ios::binary);
            vector<char> imageData{istreambuf_iterator<char>(imageFile), istreambuf_iterator<char>{}};
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
    
    file.save(TagLib::MPEG::File::AllTags);

    // rename file
    std::string ext = songName.substr(songName.find_last_of("."));
    rename((currentPath + songName).c_str(), (currentPath + (complexName ? ((metadata->TrackNumber.size() == 2 ? metadata->TrackNumber : "0" + metadata->TrackNumber) + " - " + metadata->ArtistSortName + " - " + metadata->Title) : metadata->Title) + ext).c_str());
}
