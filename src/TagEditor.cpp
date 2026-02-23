#include "TagEditor.hpp"
#include <iostream>

TagEditor::TagEditor(){}
TagEditor::~TagEditor(){}

int TagEditor::getsize(FILE* file){
    //returns the size of the file as an int
	int loc = ftell(file);
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file, loc, SEEK_SET);
	return size;
}

bool TagEditor::checkmp3(std::string filename){
    //Checks if the file extension is mp3
	int filetype = filename.length() - 4;
	std::string filetipe = filename.substr(filetype, 4);
	if (filetipe == ".mp3")
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string TagEditor::ReadThirty(FILE* file, int readloc){
    //Used to read Title, Artist, Album, Comment
	//Each is 30 bytes long
	char magic[30];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 30, file);
	std::string str(magic);
	return str;
}

std::string TagEditor::Header(FILE *file, int readloc){
    //Checks for ID3v1 Tag Header 4bytes long...
	//currently only supports v1
	//v1+ coming later
	char magic[4];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 4, file);
	std::string str(magic);
	std::string mag = str.substr(0,3);
	return mag;
}

std::string TagEditor::Year(FILE *file, int readloc){
    //Reads Year Of Track...4bytes long
	char magic[4];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 4, file);
	std::string str(magic);
	return str;
}

void TagEditor::ReadMP3(FILE *file, TagEditor::MP3Tag tag){
    //The following code reads the information for x bytes and then increases the readlocation x spaces
	//so the next information can be read
	int readloc = getsize(file)-125;
	tag.Title = ReadThirty(file, readloc);
	readloc+=30;
	tag.Artist = ReadThirty(file, readloc);
	readloc+=30;
	tag.Album = ReadThirty(file, readloc);
	readloc+=30;
	tag.Year = Year(file, readloc);
	readloc+=4;
	tag.Comment = ReadThirty(file, readloc);
	std::cout << "Artist: " << tag.Artist << std::endl;
	std::cout << "Title: " << tag.Title << std::endl;
	std::cout << "Album: "<< tag.Album << std::endl;
	std::cout << "Year: "<< tag.Year << std::endl;
}

void TagEditor::WriteArtist(FILE *file, int filesize, char Artist[]){
    
	int writeloc = filesize-95;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Artist, 1, 30, file); //goes to the storage of Artist data and writes new data
	std::cout << Artist << " set as The Artist" << std::endl;
}

void TagEditor::WriteTitle(FILE *file, int filesize, char Title[]){
    int writeloc = filesize-125;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Title, 1, 30, file); //goes to the storage of Title data and writes new data
	std::cout << Title << " set as The Title" << std::endl;
}


void TagEditor::WriteAlbum(FILE *file, int filesize, char Album[]){
    int writeloc = filesize-65;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Album, 1, 30, file); //goes to the storage of Album data and writes new data
	std::cout << Album << " set as The Album" << std::endl;
}


void TagEditor::WriteYear(FILE *file, int filesize, char Year[]){
    int writeloc = filesize-35;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Year, 1, 4, file); //goes to the storage of Year data and writes new data
	std::cout << Year << " set as The Release Year" << std::endl;
}


void TagEditor::WriteComment(FILE *file, int filesize, char Comment[]){
    int writeloc = filesize-31;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Comment, 1, 30, file); //goes to the storage of Comment data and writes new data
	std::cout << Comment << " set as The Comment" << std::endl;
}
