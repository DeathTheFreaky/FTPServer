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
#include "File.h"

namespace Server{

class Filemanager {
private:
	std::string baseDir;
protected:

public:
	Filemanager(std::string baseDir);
	virtual ~Filemanager();

	bool isValid();
	std::vector<std::string>* getFilelist();
	File* getFile(std::string);
};

}

#endif /* FILEMANAGER_H_ */
