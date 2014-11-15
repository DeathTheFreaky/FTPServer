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
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include "File.h"

class Filemanager {
private:
	std::string *baseDir;
protected:

public:
	Filemanager(std::string *baseDir);
	virtual ~Filemanager();

	bool isValid();
	std::vector<std::string>* getFilelist();
	File* getFile(std::string *fileName, bool read);
	bool fileExists(std::string *fileName);
	void deleteFile(std::string *fileName);
};

#endif /* FILEMANAGER_H_ */
