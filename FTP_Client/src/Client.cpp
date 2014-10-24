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

void Client::clientStart() {
	std::cout << "DEBUG: starting client..." << endl;
	Socket *socket = new Socket(ipAdress, port);
	std::cout << "DEBUG: created socket" << endl;
	std::string input = "";
	if (socket->conn() == true) {
		std::cout << "DEBUG: established connection" << endl;
		while (input.compare("QUIT") != 0) {
			std::cout << "Please enter command: " << endl;
			std::cin >> input;
			socket->sendCommand(input);


		}
		socket->closeSocket();
		return;
	}
	std::cout << "DEBUG: could not connect socket" << std::endl;
	return;
}
