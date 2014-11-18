/*
 * File.cpp
 *
 *  Created on: 15.11.2014
 *      Author: clemens
 */

#include "File.h"

/**
 * File is managing the file which is opend by it.
 * Parameters:
 * 		std::string *fileName: reference to the filename
 * 		bool read: defines if the file is open for read(true) or write(false)
 */
File::File(std::string *fileName, bool read) {
	std::mutex mtx;
	mtx.lock();
	if(read){
		this->file.open(fileName->c_str(), std::fstream::in | std::fstream::binary);
	}else{
		this->file.open(fileName->c_str(), std::fstream::out | std::fstream::binary);
	}
	mtx.unlock();
}

File::File(){
}

/**
 * closing the filestream
 */
File::~File() {
	if(this->file.is_open())
		this->file.close();
}

/**
 * The public method open opens a file
 * Parameters:
 * 		std::string *fileName: reverence to filename
 * 		bool read: defines opening the file for read(true) or write(false)
 */
bool File::open(std::string *fileName, bool read){
	std::mutex mtx;
	mtx.lock();
	if(read){
		this->file.open(fileName->c_str(), std::fstream::in | std::fstream::binary);
	}else{
		this->file.open(fileName->c_str(), std::fstream::out | std::fstream::binary);
	}
	mtx.unlock();
	return this->file.is_open();
}

/**
 * The public method isopen returns true if the file is open
 * Return:
 * 		bool: true if file is open
 */
bool File::isopen(){
	return this->file.is_open();
}

/**
 * The public method getLenght returns the lenght of the file
 * Return:
 * 		unsigned long: length of the file
 */
unsigned long File::getLength(){
	if(this->file.is_open()){
		this->file.seekg(0, this->file.end);
		unsigned long msglength = file.tellg();
		this->file.seekg(0, this->file.beg);
		return msglength;
	}
	return 0;
}

/**
 * The public method close closes the stream
 */
void File::close(){
	if(this->file.is_open())
		this->file.close();
}

/**
 * The public method readNextBytes reads the next Bytes of the filestream and
 * writes them into the passed char pointer.
 * Parameter:
 * 		char* bytes: reverence to a char array
 * 		int maxLength: the lenght of the array
 */
void File::readNextBytes(char* bytes, int maxLength){
	this->file.read(bytes,maxLength);
}

/**
 * The public method writeNextBytes writes the next Bytes into the filestream.
 * Parameters:
 * 		char* bytes: reverence to a char array where the bytes to write are in
 * 		int lenght: the lenght to write from the char array
 */
void File::writeNextBytes(char* bytes, int length){
	this->file.write(bytes, length);
}
