/*
 * Socket.cpp
 *
 *  Created on: 17.10.2014
 *      Author: Alexander Benesch
 */

#include <iostream>

#include "Socket.h"

#define BUF 1024

Socket::Socket(std::string ip, int port) {
	this->ip = ip;
	this->port = port;
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

bool Socket::conn() {
	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Socket error" << std::endl;
		return false;
	}

	std::cout << "DEBUG: begin connection..." << std::endl;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_aton(ip.c_str(), &address.sin_addr);
	std::cout << "DEBUG: memset - inet_aton" << std::endl;

	if(connect(create_socket, (struct sockaddr*) &address, sizeof(address)) == 0) {
		std::cout << "Connection with server " << inet_ntoa (address.sin_addr) << " established" << std::endl;
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

void Socket::closeSocket() {
	close(create_socket);
}

void Socket::sendCommand(std::string comm) {

}

int Socket::putData() {

	return 0;
}

int Socket::getData() {

	return 0;
}
