/*
 * Filemanager.h
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include "File.h"

class Filemanager {
private:
	std::string *baseDir;
	DIR *dir;
protected:

public:
	Filemanager(std::string *baseDir);
	virtual ~Filemanager();

	bool isValid();
	std::vector<std::string>* getFilelist();
	File* getFile(std::string);
};

#endif /* FILEMANAGER_H_ */
