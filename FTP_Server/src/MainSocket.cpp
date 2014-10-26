/*
 * Socket.cpp
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#include "MainSocket.h"

/**
 * The main socket gets created and binded.
 * The server starts to listen to the socket but doesn't accept clients yet.
 * Parameter:
 * 		port: The port the server should listen at
 */
MainSocket::MainSocket(int port) {
	this->port = port;
	this->s = socket(AF_INET, SOCK_STREAM, 0);

	memset(&this->address,0,sizeof(this->address));
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons (this->port);

	if (bind ( this->s, (struct sockaddr *) &this->address, sizeof (this->address)) != 0) {
		std::cerr << "Error while binding the socket." << std::endl;
		exit(4);
	}

	listen (this->s, 10);

	this->addrlen = sizeof (struct sockaddr_in);
}

MainSocket::~MainSocket() {
	close(this->s);
}

/**
 * The server accepts the next client.
 * Parameter:
 * 		cliaddress: a pointer to the pointer of the struct where the client address information should be put
 */
int MainSocket::sAccept(struct sockaddr_in **cliaddress){
	return accept(this->s, (struct sockaddr *) *cliaddress, &this->addrlen);
}

