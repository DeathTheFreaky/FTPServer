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
	this->fileName = "";
	this->status = 0;
}

Socket::~Socket() {
	this->quit();
}

/**
 * Use:
 * 		used to connect to Server
 *
 * Parameter:
 * 		none
 */
bool Socket::conn() {
	char buffer[BUF] = {};

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {	// error when trying to create socket??
		std::cerr << "Socket error" << std::endl;
		return false;
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_aton(ip.c_str(), &address.sin_addr);

	std::cout << "Connecting to server ..." << std::endl;

	// trying to connect to Server
	if(connect(create_socket, (struct sockaddr*) &address, sizeof(address)) == 0) {
		std::cout << "Connection with server " << inet_ntoa (address.sin_addr) << " established" << std::endl << std::endl;
		int recvd = 0;
		std::string welcome = "";
		while (recvd != BUF) {
			size = recv(create_socket, &buffer, BUF-1, 0);		// reading from stream / WELCOMEMESSAGE
			if(size > 0) {
				buffer[size]= '\0';
				welcome.append(buffer);
			}
			recvd += size;
		}
		std::cout << welcome << std::endl;
	} else {		// couldn't connect to server socket
		std::cerr << "Connect error - no server available" << std::endl;
		return false;
	}
	return true;
}

/**
 * Use:
 * 		closes client-socket
 *
 * Parameter:
 * 		none
 */
void Socket::closeSocket() {
	close(create_socket);
}

/**
 * Use:
 * 		used to send command to server
 *
 * Parameter:
 * 		comm: command to be sent
 */
void Socket::sendCommand(std::string comm) {
	send(this->create_socket, comm.c_str(), comm.length(), 0);
}

/**
 * Use:
 * 		used to receive answer from server
 *
 * Parameter:
 * 		none
 */
void Socket::receiveAnswer() {
	status = 0;

	recv(create_socket, &status, 1, 0);
	status = status-48;			// setting status
	return;
}

/**
 * Use:
 * 		writing files to the Server
 *
 * Parameters:
 * 		none
 */
void Socket::putData() {
	std::cout << "putData" << std::endl;
	char error = '5';
	std::string _mes = "";
	char message[BUF] = {};
	std::ifstream is;
	int percent = 0;

	int s = recv(create_socket, &message, BUF-1, 0);	// receiving name of file
	if (s > 0) {
		fileName.assign(message);
		status = atoi(fileName.substr(0, 1).c_str());
		fileName = fileName.substr(fileName.find_first_of(' ')+1);
	}

	is.open(fileName.c_str(), std::ifstream::in | std::ifstream::binary);	// open file to read
	if (is.is_open()) {
		//get length of file:
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);

		std::stringstream ss;
		ss << length;

		// constructing message for server
		_mes.assign("3 ");
		_mes.append(ss.str());

		send(this->create_socket, _mes.c_str(), _mes.length(), 0);

		recv(create_socket, &status, 1, 0); // receive new status

		if (status != '3') {	// if file can't be sent to server...
			err();
			is.close();
			return;
		}

		// read and send data as block:
		int sent = 0;
		while (sent < length) {

			is.read (message, 1024);

			if (length - sent >= 1024) {
				sent += send(this->create_socket, message, 1024, 0);
			} else {
				sent += send(this->create_socket, message, length-sent, 0);
			}
			percent = (sent*100)/length;
			std::cout << "\r " << percent << "% finished";
		}
		// file has been sent

		std::cout << std::endl << "Upload finished!" << std::endl;
	} else {
		std::cout << "File can't be opened or doesn't exist" << std::endl;
		send(this->create_socket, &error, sizeof(error), 0);
	}
	is.close();
	return;
}

/**
 * Use:
 * 		getting Files from the server
 *
 * Parameters:
 * 		none
 */
void Socket::getData() {
	char get = '2';
	char length[BUF] = {};
	int received = 0;
	char fileContent[BUF];
	int success = 1;
	int percent = 0;

	int s = recv(create_socket, &length, BUF-1, 0);	// receiving name and length of file
	if(s > 0) {
		length[s] = '\0';
		fileName.assign(length);
		std::string _length = fileName.substr(fileName.find_first_of(' ')+1);
		fileName.assign(_length.substr(_length.find_first_of(' ')+1));
		_length = _length.substr(0, _length.find_first_of(' '));
		len = atoi(_length.c_str());
	}

	send(this->create_socket, &get, 1, 0); // sending '2' to server - meaning: ready to get File

	// Open file to write
	std::ofstream out;
	out.open(fileName.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	// receiving File
	while (received < len) {
		s = recv(create_socket, fileContent, BUF, 0);	// receiving block of data

		if (s > 0) {
			out.write(fileContent, s);		// writing block into file
		} else {
			std::cout << "There is no such file" << std::endl;
			success = 0;
			break;
		}
		received += s;
		percent = (received*100)/len;
		std::cout << "\r " << percent << "% finished";
	}
	out.close();

	if (success) {
		std::cout << std::endl << "Download finished!" << std::endl;
	} else {
		std::cout << std::endl << "File couldn't be downloaded!" << std::endl;
	}
	return;
}

/**
 * Use:
 * 		used to get and display list of files from server
 *
 * Parameter:
 * 		none
 */
void Socket::showList() {
	char message[BUF] = {};
	char list = '1';
	char length[BUF] = {};

	int s = recv(create_socket, &length, BUF-1, 0);	// receiving length of list
	if (s > 0) {
		length[s] = '\0';
		fileName.assign(length);
		std::cout << fileName << std::endl;
		std::string length = fileName.substr(0);
		len = atoi(length.c_str());
	}

	send(this->create_socket, &list, 1, 0);		// sending '1' to tell server to send the list

	s = recv(create_socket, &message, len, 0);	// receiving actual list
	if(s > 0) {
		message[s]= '\0';
		std::cout << message;
	}
	return;
}

/**
 * Use:
 * 		quitting socket by closing it
 *
 * Parameter:
 * 		none
 */
void Socket::quit() {
	closeSocket();
}

/**
 * Use:
 * 		displaying error messages from server
 *
 * Parameter:
 * 		none
 */
void Socket::err() {
	char err = '5';
	char length[BUF] = {};

	int s = recv(create_socket, &length, BUF-1, 0);	// receiving length of error message
	if (s > 0) {
		length[s] = '\0';
		fileName.assign(length);
		std::string length = fileName.substr(0);
		len = atoi(length.c_str());
	}
	char message[len];

	send(this->create_socket, &err, 1, 0);				// telling server to send actual error message
	int size = recv(create_socket, &message, len, 0);	// receiving actual error message
	if(size > 0) {
		message[size]= '\0';
		std::cout << message;
	}
}

/**
 * Use:
 * 		used to login the client
 *
 * Parameter:
 * 		none
 */
void Socket::login() {		// finished
	std::string user = "";
	std::string pass = "";
	std::string login = "";

	while(user.empty()) {		// forcing user to enter username
		std::cout << "Please enter username: ";
		getline(std::cin, user);
		if(user.empty()) std::cout << "Incorrect username!" << std::endl;
	}
	while(pass.empty()) {		// forcing user to enter passwort
		pass = getpass("Please enter password: ");	// getpass() hides user-input
		if(pass.empty()) std::cout << "Incorrect password!" << std::endl;
	}
	std::cout << std::endl;

	// constructing login message for server
	login.assign("0 ");
	login.append(user);
	login.append(" ");
	login.append(pass);

	send(this->create_socket, login.c_str(), login.length(), 0);	// sending login message to server

	recv(create_socket, &status, 1, 0); //get status info if login was successful

	// if login was unsuccessful, err() is called
	if (status != '0') {
		err();
		return;
	}

	std::cout << "Sucessfully logged in! =)" << std::endl << std::endl;
}

/**
 * Use:
 * 		returns statusCode
 *
 * Parameter:
 * 		none
 */
int Socket::getStatus() {
	return this->status;
}
