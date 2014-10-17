/*
 * Filemanager.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "Filemanager.h"

namespace Server{

Filemanager::Filemanager(std::string baseDir) {
	this->baseDir = opendir(baseDir.c_str());
}

Filemanager::~Filemanager() {
	// TODO Auto-generated destructor stub
}

}
