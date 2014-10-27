/*
 * File.h
 *
 *  Created on: 17.10.2014
 *      Author: Alexander Benesch
 */

#include <string>

#ifndef FILE_H_
#define FILE_H_



class File {
public:
	File();
	virtual ~File();

private:
	std::string name;
	int size;
	FILE *file;
};

#endif /* FILE_H_ */
