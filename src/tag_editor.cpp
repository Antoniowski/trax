#include "tag_editor.hpp"
#include "taglib/tag.h"
#include "taglib/fileref.h"
#include "taglib/id3v2tag.h"
#include "taglib/attachedpictureframe.h"
#include "taglib/mpegfile.h"
#include <fstream>

using namespace std;

void editTags(vector<string> songNames, string songsDirPath,  vector<MetadataSearcher::MP3Tag>* metadatas, string artistName)
{
    for(int i = 0; i < songNames.size(); i++)
    {
        TagLib::MPEG::File file((songsDirPath+songNames[i]).c_str());
        if(file.isValid())
            continue;
        MetadataSearcher::MP3Tag tag;
        bool songFound = false;
        for(auto t : *metadatas)
        {
            string fileNameStr = songNames[i];
            string songNameStr = t.Title;

            //lowercase + trim + normalize + remove spaces
            transform(fileNameStr.begin(), fileNameStr.end(), fileNameStr.begin(), ::tolower);
            fileNameStr.erase(remove_if(fileNameStr.begin(), fileNameStr.end(), [](char c) {
                return !isalnum(c) && c != ' ';
                }), fileNameStr.end());
            fileNameStr.erase(0, fileNameStr.find_first_not_of(" \t\r\n"));
            fileNameStr.erase(fileNameStr.find_last_not_of(" \t\r\n") + 1);
            fileNameStr.erase(remove_if(fileNameStr.begin(), fileNameStr.end(), [](char c){
                return isspace(c);
            }), fileNameStr.end());
            
            //lowercase + trim + normalize + remove spaces
            transform(songNameStr.begin(), songNameStr.end(), songNameStr.begin(), ::tolower);
            songNameStr.erase(remove_if(songNameStr.begin(), songNameStr.end(), [](char c) {
                return !isalnum(c) && c != ' ';
                }), songNameStr.end());
            songNameStr.erase(0, songNameStr.find_first_not_of(" \t\r\n"));
            songNameStr.erase(songNameStr.find_last_not_of(" \t\r\n") + 1);
            songNameStr.erase(remove_if(songNameStr.begin(), songNameStr.end(), [](char c){
                return isspace(c);
            }), songNameStr.end());

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

        file.tag()->setTitle((tag.Title.empty() || tag.Title == " ") ? string("song_" + to_string(i)) : tag.Title);
        file.tag()->setAlbum(tag.Album);
        file.tag()->setArtist(artistName);
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
        
        // edit image
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
            cout << "Error during image download. Skipped." << endl;
        }
        
        file.save();

        // rename file
        rename((songsDirPath + songNames[i]).c_str(), (songsDirPath + tag.Title + ".mp3").c_str());
    }
}