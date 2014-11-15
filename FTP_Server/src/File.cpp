/*
 * File.cpp
 *
 *  Created on: 15.11.2014
 *      Author: clemens
 */

#include "File.h"

File::File(std::string *fileName, bool read) {
	if(read){
		this->file.open(fileName->c_str(), std::fstream::in | std::fstream::binary);
	}else{
		this->file.open(fileName->c_str(), std::fstream::out | std::fstream::binary);
	}
}

File::File(){
}

File::~File() {
	if(this->file.is_open())
		this->file.close();
}

bool File::open(std::string *fileName, bool read){
	std::cout << "try open: " << *fileName << std::endl;
	if(read){
		this->file.open(fileName->c_str(), std::fstream::in | std::fstream::binary);
	}else{
		this->file.open(fileName->c_str(), std::fstream::out | std::fstream::binary);
	}
	return this->file.is_open();
}

bool File::isopen(){
	return this->file.is_open();
}

unsigned long File::getLength(){
	if(this->file.is_open()){
		this->file.seekg(0, this->file.end);
		unsigned long msglength = file.tellg();
		this->file.seekg(0, this->file.beg);
		return msglength;
	}
	return 0;
}

void File::close(){
	if(this->file.is_open())
		this->file.close();
}

void File::readNextBytes(char* bytes, int maxLength){
	this->file.read(bytes,maxLength);
}

void File::writeNextBytes(char* bytes, int length){
	this->file.write(bytes, length);
}
