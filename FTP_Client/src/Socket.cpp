/*
 * Socket.cpp
 *
 *  Created on: 17.10.2014
 *      Author: Alexander Benesch
 */

#include <iostream>

#include "Socket.h"

Socket::Socket(std::string ip, int port) {
	this->ip = ip;
	this->port = port;
	this->size = 0;
	this->create_socket = 0;
	this->status = 0;
	this->len = 0;
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

bool Socket::conn() {
	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Socket error" << std::endl;
		return false;
	}

	//std::cout << "DEBUG: begin connection..." << std::endl;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_aton(ip.c_str(), &address.sin_addr);

	std::cout << "Connecting to server ..." << std::endl;

	if(connect(create_socket, (struct sockaddr*) &address, sizeof(address)) == 0) {
		std::cout << "Connection with server " << inet_ntoa (address.sin_addr) << " established" << std::endl << std::endl;
		size = recv(create_socket, &buffer, BUF-1, 0);
		if(size > 0) {
			buffer[size]= '\0';
	        std::cout << buffer;
		}
	} else {
		std::cerr << "Connect error - no server available" << std::endl;
		return false;
	}
	return true;
}

void Socket::closeSocket() {	// finished
	close(create_socket);
}

void Socket::sendCommand(std::string comm) {	// finished
	send(this->create_socket, comm.c_str(), comm.length(), 0);
}

void Socket::receiveAnswer() {	// work in progress
	char length[BUF] = {};
	//char space = '*';

	recv(create_socket, &status, 1, 0); // ??
	status = status-48;
	std::cout << "recvAns: DEBUG - STATUS: " << status << std::endl;

	//recv(create_socket, &space, 1, 0);	// sollte theoretisch das leerzeichen auslesen, damit es später nicht verwendet wird
	//std::cout << "DEBUG - space:" << space << ":" << std::endl;

	/*do {
		int size = recv(create_socket, &buffer, BUF-1, 0);
		if(size > 0) {
			buffer[size]= '\0';
			std::cout << buffer;
		}
	} while (size == 1023);*/

	//dat.assign(buffer);
	//std::cout << "DEBUG: status - " << status << "   data - "<< dat << std::endl;

	int s = recv(create_socket, &length, BUF-1, 0);	// BUF-1 müsste ausreichen, da es nur eine Zahl sein sollte ;)
	if(s > 0) {
		length[s]= '\0';
		len = atoi(length);
		std::cout << "recvAns: DEBUG: Laenge des Inhalts: " << len << std::endl;
	}
}

int Socket::getStatus() {	// finished
	return status;
}

int Socket::getLength() {	// finished
	return len;
}

void Socket::putData() {	// work in progress
	char put = '3';
	char message[BUF];

	// code = 3

	int s = recv(create_socket, &message, BUF-1, 1);
	if (s == 1 && message[0] == '3') {
		// Client: sende code3 + länge d. Datei als Antwort


		// Server: code3 heißt empfangsbereit


		// Client: Datei senden

	} else {
		std::cerr << "put: ERROR: An unknown error occured" << std::endl;
	}
	return;
}

void Socket::getData(int len) {	// work in progress
	char get = '2';
	// code = 2, länge = len


	// Client: sendet code 2 als Antwort
	send(this->create_socket, &get, 1, 0);


	// Server: sendet Datei


	// Client: speichert datei in aktuellem Ordner

	return;
}

void Socket::showList() {	// work in progress?
	char message[BUF];
	char list = '1';

	send(this->create_socket, &list, 1, 0);

	int s = recv(create_socket, &message, len, 0);
	if(s > 0) {		//ne while schleife drumherum weil nachricht zu lang sein könnte??
		message[size]= '\0';
		std::cout << message;
	}
	return;
}

void Socket::quit() {	// finished
	closeSocket();
}

void Socket::err() {	// finished??
	char err = '5';
	//char _len[BUF];
	//recv(create_socket, &_len, BUF-1, 0);
	//len = atoi(_len);
	char message[len];
	std::cout << "err: DEBUG: len = " << len << std::endl;
	send(this->create_socket, &err, 1, 0);
	std::cout << "err: DEBUG: send successful" << std::endl;
	int size = recv(create_socket, &message, len, 0); // ??
	std::cout << "err: DEBUG: recv successful" << std::endl;
	if(size > 0) {
		std::cout << "err: DEBUG: size = " << size << std::endl;
		message[size]= '\0';
		std::cout << message;
	}
}
