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
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

void Client::clientStart() {
	//std::cout << "DEBUG: starting client..." << endl;
	Socket *socket = new Socket(ipAdress, port);
	//std::cout << "DEBUG: created socket" << endl;
	std::string input = "";
	if (socket->conn() == true) {
		//std::cout << "DEBUG: established connection" << endl;
		while (!quit) {
			std::cout << "Please enter command: ";
			std::getline(std::cin, input);	//zeilenweises einlesen der Eingabe
			//std::cout << "DEBUG-Before Send: " << input << std::endl;
			socket->sendCommand(input);
			socket->receiveAnswer();

			switch(socket->getStatus()) {
			case 1:	// LIST
				socket->showList();
				break;
			case 2: // GET
				socket->getData(socket->getLength()); // length?
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
		socket->closeSocket();	// is das überhaupt möglich, weil wegen socket->quit() ?? o.O
		return;
	}
	std::cout << "clientStart: DEBUG: could not connect socket" << std::endl;
	return;
}
