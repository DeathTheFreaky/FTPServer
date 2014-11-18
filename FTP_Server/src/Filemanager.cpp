/*
 * Filemanager.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "Filemanager.h"

/**
 * The Filemanager is the interface to the filesystem.
 * Parameters:
 * 		std::string *baseDir: a reverence to a the base directory path
 */
Filemanager::Filemanager(std::string *baseDir) {
	this->baseDir = baseDir;
	DIR *dir;
	//try to open directory
	if ((dir = opendir(this->baseDir->c_str())) == NULL) {
		std::cerr << "Error while opening directory: " << dir << std::endl;
		exit(6);
	}
	closedir(dir);
}

Filemanager::~Filemanager() {

}

/**
 * The public method getFilelist returns a list of all files in the download directory
 * Return:
 * 		std::vector<std::string>*: the reverence to the filelist
 */
std::vector<std::string>* Filemanager::getFilelist() {
	std::vector<std::string> *fileList = new std::vector<std::string>;
	struct stat stats;
	struct dirent *direntry;
	DIR *dir;

	//open the directory
	if ((dir = opendir(this->baseDir->c_str())) == NULL) {
		std::cerr << "Error while opening directory: " << dir << std::endl;
		exit(4);
	}

	//read every directory entry
	while ((direntry = readdir(dir)) != NULL) {
		stat(direntry->d_name, &stats);
		if (direntry->d_type == DT_REG || direntry->d_type == DT_UNKNOWN)
			fileList->push_back(std::string(direntry->d_name));
	}
	closedir(dir);
	return fileList;
}

/**
 * The public method getFile returns a reverence to the file.
 * Parameters:
 * 		std::string *fileName: a reverence to the FileName
 * 		bool read: defines the opening method for the file read(true) or write(false)
 * Return:
 * 		File*: a reverence to the file or NULL if the file does not exist
 */
File* Filemanager::getFile(std::string *fileName, bool read) {
	File *file = new File();
	std::string fullFileName;

	//generate the full Filepath
	fullFileName.append(*this->baseDir);
	fullFileName.append("/");
	fullFileName.append(*fileName);

	//opens the file
	if (file->open(&fullFileName, read)) {
		return file;
	}
	delete file;
	return NULL;
}

/**
 * The public method fileExists checks if a file exists
 * Parameters:
 * 		std::string *fileName: a reverence to the fileName
 * Return:
 * 		bool: returns if the file exists
 */
bool Filemanager::fileExists(std::string *fileName) {
	std::vector<std::string> *fileList = getFilelist();
	for (unsigned int i = 0; i < fileList->size(); i++) {
		if (fileList->at(i).compare(*fileName) == 0) {
			return true;
		}
	}
	return false;
}

/**
 * The public method deleteFile delets a file in the directory it is used if something strange happens
 * Parameters:
 * 		std::string *fileName: revernece to the fileName
 */
void Filemanager::deleteFile(std::string *fileName) {
	std::string file;
	file.append(*this->baseDir);
	file.append("/");
	file.append(*fileName);
	remove(file.c_str());
}
