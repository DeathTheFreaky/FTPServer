/*
 * Client.cpp
 *
 *  Created on: 10.10.2014
 *      Author: Alexander Benesch
 */

#include "Client.h"

Client::Client(std::string ip, int port) {
	this->ipAdress = ip;
	this->port = port;
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

int Client::clientStart() {
	Socket *socket = new Socket(ipAdress, port);



	return 0;
}
