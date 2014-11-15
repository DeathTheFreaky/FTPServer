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
	this->quit = false;
	this->socket = new Socket(ipAdress, port);
}

Client::~Client() {
	delete this->socket;
}

void Client::clientStart() {
	std::string input = "";
	if (socket->conn() == true) {
		while (!quit) {
			std::cout << "Please enter command: " << std::endl;
			input.clear();
			while(input.empty()) {
				std::getline(std::cin, input);
			}
			socket->sendCommand(input);
			std::cout << std::endl;
			socket->receiveAnswer();

			switch(socket->getStatus()) {
			case 0: // LOGIN
				socket->login();
				break;
			case 1:	// LIST
				socket->showList();
				break;
			case 2: // GET
				socket->getData();
				break;
			case 3:	// PUT
				socket->putData();
				break;
			case 4:	// QUIT
				socket->quit();
				quit = true;
				break;
			case 5:	// ERROR
				socket->err();
				break;
			default:
				std::cerr << "An unknown error occured" << std::endl;
				break;
			}
		}
		std::cout << "Client successfully shut down." << std::endl;
		return;
	}
	std::cout << "clientStart: DEBUG: could not connect socket" << std::endl;
	return;
}
