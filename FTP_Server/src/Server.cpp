/*
 * Server.cpp
 *
 *  Created on: 09.10.2014
 *      Author: Clemens Grabmann
 */

#include "Server.h"

namespace Server{

Server::Server(int port, std::string dir) {
	this->mainSocket = new Socket(port);
	this->filemanager = new Filemanager(dir);
}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

void Server::start(){
	//TODO start for the server
}

}
