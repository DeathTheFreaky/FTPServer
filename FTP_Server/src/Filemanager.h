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
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>

class Filemanager {
private:
	std::string *baseDir;
protected:

public:
	Filemanager(std::string *baseDir);
	virtual ~Filemanager();

	bool isValid();
	std::vector<std::string>* getFilelist();
	std::fstream* getFile(std::string *fileName);
	bool fileExists(std::string *fileName);
	void deleteFile(std::string *fileName);
	std::fstream* openFile(std::string *fileName);
};

#endif /* FILEMANAGER_H_ */
