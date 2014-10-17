/*
 * Server.cpp
 *
 *  Created on: 09.10.2014
 *      Author: Clemens Grabmann
 */

#include "Server.h"

namespace Server{

Server::Server(int port, std::string dir) {
	this->mainSocket = new MainSocket(port);
	this->filemanager = new Filemanager(dir);
}

Server::~Server() {
	delete this->mainSocket;
	delete this->filemanager;
}

void Server::start(){
	ConnectionSocket *conn;
	struct sockaddr_in *cliaddress;
	while(true){
		conn = new ConnectionSocket(this->mainSocket->sAccept(cliaddress));
	}
}

}
