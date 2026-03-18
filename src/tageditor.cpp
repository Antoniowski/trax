#include "tageditor.hpp"
#include "MetadataSearcher.hpp"
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

void editTags(vector<string> songNames, string songsDirPath, vector<MetadataSearcher::MP3Tag>* metadatas, string artistName, bool noImage)
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

        file.tag()->setTitle(TagLib::String((tag.Title.empty() || tag.Title == " ") ? string("song_" + to_string(i)) : tag.Title, TagLib::String::UTF8));
        file.tag()->setAlbum(TagLib::String(tag.Album, TagLib::String::UTF8));
        file.tag()->setArtist(TagLib::String(artistName, TagLib::String::UTF8));
        try 
        {
            file.tag()->setTrack(stoi(tag.TrackNumber));
        }
        catch (exception e) 
        {
            file.tag()->setTrack(i);
        }
        try 
        {
            file.tag()->setYear(stoi((tag.Year).substr(0, 4)));
        } 
        catch (exception e) 
        {
            file.tag()->setYear(0);
        }

        file.tag()->setGenre(TagLib::String(tag.Genre, TagLib::String::UTF8));
        
        // edit image
        if(!noImage)
        {
            try
            {
                ifstream imageFile(string("./" + tag.AlbumID + "-front.jpg"), ios::binary);
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
            catch(exception e )
            {
                cout << "[WARNING] Cover art attachment skipped." << endl;
            }
        }
        
        file.save();

        // rename file
        std::string ext = songNames[i].substr(songNames[i].find_last_of("."));
        rename((songsDirPath + songNames[i]).c_str(), (songsDirPath + tag.Title + ext).c_str());
    }
}