/*
 * ConnectionSocket.cpp
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#include "ConnectionSocket.h"

namespace Server {

ConnectionSocket::ConnectionSocket(int socketID, Filemanager *filemanager) {
	this->socketID = socketID;
	this->filemanager = filemanager;
}

ConnectionSocket::~ConnectionSocket() {
	// TODO Auto-generated destructor stub
}

}
