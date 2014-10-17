/*
 * File.h
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>

namespace Server {

class File {
private:
	FILE *file;
	std::string *name;
	int size;
protected:

public:
	File(std::string *name);
	virtual ~File();
};

}

#endif /* FILE_H_ */
