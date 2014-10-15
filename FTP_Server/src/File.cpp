/*
 * File.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "File.h"

namespace Server {

File::File(std::string *name) {
	this->name = name;
	this->file = NULL;
	this->size = 0;
}

File::~File() {
	// TODO Auto-generated destructor stub
}

} /* namespace Server */
