/*
 * Filemanager.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "Filemanager.h"

Filemanager::Filemanager(std::string *baseDir) {
	this->baseDir = baseDir;
	if((this->dir  = opendir(this->baseDir->c_str())) == NULL) {
		std::cerr << "Error while opening directory: " << dir << std::endl;
		exit(4);
	}
}

Filemanager::~Filemanager() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string>* Filemanager::getFilelist(){
	std::vector<std::string> *fileList = new std::vector<std::string>;
	struct stat stats;
	struct dirent *direntry;
	while ((direntry = readdir(this->dir)) != NULL) {
		stat(direntry->d_name, &stats);
		if(stats.st_mode & S_IFREG )
			fileList->push_back(std::string(direntry->d_name));
	}
	return fileList;
}
