/*
 * File.h
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>

class File {
private:
	std::string *name;
protected:

public:
	File(std::string *name);
	virtual ~File();
};

#endif /* FILE_H_ */
