/*
 * Filemanager.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "Filemanager.h"

Filemanager::Filemanager(std::string *baseDir) {
	this->baseDir = baseDir;
	DIR *dir;
	if ((dir = opendir(this->baseDir->c_str())) == NULL) {
		std::cerr << "Error while opening directory: " << dir << std::endl;
		exit(6);
	}
	closedir(dir);
}

Filemanager::~Filemanager() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string>* Filemanager::getFilelist() {
	std::vector<std::string> *fileList = new std::vector<std::string>;
	struct stat stats;
	struct dirent *direntry;
	DIR *dir;
	if ((dir = opendir(this->baseDir->c_str())) == NULL) {
		std::cerr << "Error while opening directory: " << dir << std::endl;
		exit(4);
	}
	while ((direntry = readdir(dir)) != NULL) {
		stat(direntry->d_name, &stats);
		if (direntry->d_type == DT_REG || direntry->d_type == DT_UNKNOWN)
			fileList->push_back(std::string(direntry->d_name));
	}
	closedir(dir);
	return fileList;
}

File* Filemanager::getFile(std::string *fileName, bool read) {
	File *file = new File();
	std::string fullFileName;
	fullFileName.append(*this->baseDir);
	fullFileName.append("/");
	fullFileName.append(*fileName);
	if (file->open(&fullFileName, read)) {
		return file;
	}
	delete file;
	return NULL;
}

bool Filemanager::fileExists(std::string *fileName) {
	std::vector<std::string> *fileList = getFilelist();
	for (unsigned int i = 0; i < fileList->size(); i++) {
		if (fileList->at(i).compare(*fileName) == 0) {
			return true;
		}
	}
	return false;
}

void Filemanager::deleteFile(std::string *fileName) {
	std::string file;
	file.append(*this->baseDir);
	file.append("/");
	file.append(*fileName);
	remove(file.c_str());
}
