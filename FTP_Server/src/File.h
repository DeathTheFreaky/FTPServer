/*
 * File.h
 *
 *  Created on: 15.11.2014
 *      Author: clemens
 */

#ifndef FILE_H_
#define FILE_H_

#include <fstream>
#include <iostream>
#include <string>

class File {
private:
	std::fstream file;
protected:

public:
	File(std::string *fileName, bool read);
	File();
	virtual ~File();

	bool open(std::string *fileName, bool read);
	void readNextBytes(char* bytes, int maxLength);
	void writeNextBytes(char* bytes, int length);
	void close();
	unsigned long getLength();
	bool isopen();
};

#endif /* FILE_H_ */
