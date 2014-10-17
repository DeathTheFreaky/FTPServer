/*
 * Socket.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "MainSocket.h"

namespace Server{

MainSocket::MainSocket(int port) {
	this->port = port;
	this->socket = socket (AF_INET, SOCK_STREAM, 0);

	memset(&this->address,0,sizeof(this->address));
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons (this->port);

	if (bind ( this->socket, (struct sockaddr *) &this->address, sizeof (this->address)) != 0) {
		std::cerr << "Error while binding the socket." << std::endl;
		exit(4);
	}

	listen (this->socket, 10);

	this->addrlen = sizeof (struct sockaddr_in);
}

MainSocket::~MainSocket() {
	// TODO Auto-generated destructor stub
}

int MainSocket::sAccept(struct sockaddr_in *cliaddress){
	return accept(this->socket, (struct sockaddr *) cliaddress, &this->addrlen);
}

}
