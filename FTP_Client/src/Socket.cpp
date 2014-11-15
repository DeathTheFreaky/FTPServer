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
}

Socket::~Socket() {
	// TODO Auto-generated destructor stub
}

/**
 * Use:
 * 		used to connect to Server
 * Parameter: none
 */
bool Socket::conn() {
	char buffer[BUF] = {};

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {	// error when trying to create socket??
		std::cerr << "Socket error" << std::endl;
		return false;
	}

	//std::cout << "DEBUG: begin connection..." << std::endl;
	memset(&address, 0, sizeof(address));			//
	address.sin_family = AF_INET;					//
	address.sin_port = htons(port);					//
	inet_aton(ip.c_str(), &address.sin_addr);		//

	std::cout << "Connecting to server ..." << std::endl;

	// trying to connect to Server
	if(connect(create_socket, (struct sockaddr*) &address, sizeof(address)) == 0) {
		std::cout << "Connection with server " << inet_ntoa (address.sin_addr) << " established" << std::endl << std::endl;
		int recvd = 0;
		std::string welcome = "";
		while (recvd != BUF) {
			size = recv(create_socket, &buffer, BUF-1, 0);		// trying to read from stream / WELCOMEMESSAGE
			if(size > 0) {
				buffer[size]= '\0';
				//std::cout << "conn: DEBUG: buffer: " << buffer << std::endl;
				welcome.append(buffer);
			}
			recvd += size;
			//std::cout << "conn: DEBUG: recvd = " << recvd << std::endl;
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
 */
void Socket::closeSocket() {
	close(create_socket);
}

/**
 * Use:
 * 		used to send command to server
 * Parameter:
 * 		comm: command to be sent
 */
void Socket::sendCommand(std::string comm) {
	send(this->create_socket, comm.c_str(), comm.length(), 0);
	//std::cout << "sendComm: DEBUG: command sent" << std::endl;
}

/**
 * Use:
 * 		used to receive answer from server
 * Parameter: none
 */
void Socket::receiveAnswer() {	// work in progress
	//char length[BUF] = {};
	status = 0;

	recv(create_socket, &status, 1, 0); // ??
	status = status-48;
	//std::cout << "recvAns: DEBUG: STATUS: " << status << std::endl;

	return;
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

	/*
	int s = 0;
	switch (status) {
	case 2:
		s = recv(create_socket, &length, BUF-1, 0);	// BUF-1 müsste ausreichen, da es nur eine Zahl sein sollte ;)
		if(s > 0) {
			length[s] = '\0';
			fileName.assign(length);
			std::string _length = fileName.substr(0,fileName.find_first_of(' '));
			fileName = fileName.substr(fileName.find_first_of(' '));
			len = atoi(_length.c_str());
			std::cout << "recvAns: DEBUG: Laenge des Inhalts: " << len << std::endl;
			std::cout << "recvAns: DEBUG: Name des Files: " << fileName << std::endl;
		}
		break;
	default:
		s = recv(create_socket, &length, BUF-1, 0);	// BUF-1 müsste ausreichen, da es nur eine Zahl sein sollte ;)
		if (s > 0) {
			length[s] = '\0';
			fileName.assign(length);
			std::cout << fileName << std::endl;
			std::string length = fileName.substr(0);
			std::cout << "bla debug: " << fileName << std::endl;
			len = atoi(length.c_str());
			std::cout << "recvAns: DEBUG: Laenge des Inhalts: " << len << std::endl;
		}
		break;
	}
	*/
}

int Socket::getStatus() {	// finished
	return status;
}

int Socket::getLength() {	// finished
	return len;
}

/**
 * Use:	writing files to the Server
 *
 * Parameters: none
 */
void Socket::putData() {	// work in progress
	std::cout << "putData" << std::endl;
	//char put = '3';
	char error = '5';
	std::string _mes = "";
	char message[BUF] = {};
	std::ifstream is;
	int percent = 0;

	int s = recv(create_socket, &message, BUF-1, 0);
	//std::cout << "putData: DEBUG: s = " << s << std::endl;
	//perror("Error in putData");
	if (s > 0) {
		fileName.assign(message);
		status = atoi(fileName.substr(0, 1).c_str());
		//std::cout << "putData: DEBUG: Status: " << (char)status << std::endl;
		fileName = fileName.substr(fileName.find_first_of(' ')+1);
		//std::cout << "putData: DEBUG: Name des Files: " << fileName << std::endl;
	}

	//std::cout << "putData: DEBUG: Name des Files: " << fileName << std::endl;

	is.open(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	if (is.is_open()) {
		//get length of file:
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);

		std::stringstream ss;
		ss << length;
		//std::cout << "putData: DEBUG: stringstream: " << ss << std::endl;
		//std::cout << "putData: DEBUG: stringstream: " << ss.str() << std::endl;

		_mes.assign("3 ");
		_mes.append(ss.str());
		//std::cout << "putData: DEBUG: _mes= " << _mes << std::endl;

		send(this->create_socket, _mes.c_str(), _mes.length(), 0);
		//std::cout << "putData: DEBUG: Status + lenght sended" << std::endl;

		recv(create_socket, &status, 1, 0); // ??
		//std::cout << "putData: DEBUG: answer received - Status = " << (char)status << std::endl;

		if (status != '3') {
			//std::cout << "putData: DEBUG: wrong status" << std::endl;
			err();
			is.close();
			return;
		}

		//std::cout << "putData: DEBUG: Reading " << length << " characters... " << std::endl;

		// read and send data as block:
		int sent = 0;
		//int readDebug = 0;
		while (sent < length) {

			is.read (message, 1024);
			//readDebug = (int)is.gcount();

			//std::cout << "putData: DEBUG: All characters read successfully" << std::endl;
			//std::cout << "putData: DEBUG: Sending block..." << std::endl;
			if (length - sent >= 1024) {
				sent += send(this->create_socket, message, 1024, 0);
				//std::cout << "putData: DEBUG: sent block of 1024" << std::endl;
			} else {
				sent += send(this->create_socket, message, length-sent, 0);
				//std::cout << "putData: DEBUG: sent smaller block" << std::endl;
			}

			percent = (sent*100)/length;
			std::cout << "\r " << percent << "% finished";

			//std::cout << "Successfully sent block of data" << std::endl;

			/*
			if(!is) {
				std::cout << "Error: only " << is.gcount() << " characters could be read" << std::endl;
				break;
			}
			*/
		}
		//std::cout << "putData: DEBUG: Read " << is.gcount() << " characters" << std::endl;
		//std::cout << "putData: DEBUG: Sent " << sent << " characters" << std::endl;
		std::cout << std::endl << "Upload finished!" << std::endl;
	} else {
		std::cout << "File can't be opened or doesn't exist" << std::endl;
		send(this->create_socket, &error, sizeof(error), 0);
	}

	//send(this->create_socket, message, sizeof(message), 0);
	//std::cout << "putData: DEBUG: _mes= " << _mes << std::endl;
	is.close();
	return;
}

/**
 * Use:	getting Files from the server
 *
 * Parameters: none
 */
void Socket::getData() {	// finished
	char get = '2';
	char length[BUF] = {};
	int received = 0;
	char fileContent[BUF];
	int success = 1;
	int percent = 0;

	int s = recv(create_socket, &length, BUF-1, 0);	// BUF-1 müsste ausreichen, da es nur eine Zahl sein sollte ;)
	if(s > 0) {
		length[s] = '\0';
		fileName.assign(length);
		std::string _length = fileName.substr(fileName.find_first_of(' ')+1);
		//std::cout << "getData: DEBUG: _length = " << _length << std::endl;
		fileName.assign(_length.substr(_length.find_first_of(' ')+1));
		//std::cout << "getData: DEBUG: filename = " << fileName << std::endl;
		_length = _length.substr(0, _length.find_first_of(' '));
		len = atoi(_length.c_str());
		//std::cout << "getData: DEBUG: Laenge des Inhalts: " << len << std::endl;
		//std::cout << "getData: DEBUG: Name des Files: " << fileName << std::endl;
	}

	// Client: sendet code 2 als Antwort
	send(this->create_socket, &get, 1, 0);
	//std::cout << "getData: DEBUG: answer sent" << std::endl;

	// Datei öffnen um zu schreiben
	std::ofstream out;
	out.open(fileName.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
	//if(out.is_open()) {std::cout << "getData: DEBUG: opened File" << std::endl;}

	//std::cout << "getData: DEBUG: received = " << received << "; length = " << len << std::endl;
	// Client: empfängt Datei
	while (received < len) {
		//std::cout << "getData: DEBUG: preparing recv - len = " << len << "; received = " << received << std::endl;
		s = recv(create_socket, fileContent, BUF, 0);

		//std::cout << "getDate: DEBUG: bis hierhin und nicht weiter?" << std::endl;
		if (s > 0) {
			out.write(fileContent, s);
			//std::cout << "getData: DEBUG: write completed" << std::endl;
			//std::cout << "getData: GEBUG: oder doch?" << std::endl;
		} else {
			std::cout << "There is no such file" << std::endl;
			success = 0;
			break;
		}
		received += s;
		//std::cout << "received/len = " << (received*100)/len << "; percent = " << percent << std::endl;
		percent = (received*100)/len;
		std::cout << "\r " << percent << "% finished";
	}
	//std::cout << "getData: DEBUG: received = " << received << "; length = " << len << std::endl;
	out.close();
	if (success) {
		std::cout << std::endl << "Download finished!" << std::endl;
	} else {
		std::cout << std::endl << "File couldn't be downloaded!" << std::endl;
	}

	/*if(fs) {
		char *buffer = new char[len];

		std::cout << "Reading " << len << " characters... " << std::endl;
		// read data as a block:
		fs.read(buffer, len);

		if(fs) {
			std::cout << "All characters have been read successfully" << std::endl;
		} else {
			std::cout << "error: only " << fs.gcount() << " could be read" << std::endl;
		}
		fs.close();
	}
	*/
	return;
}

/**
 * Use:
 * 		used to get and display list of files from server
 * Parameter: none
 */
void Socket::showList() {	// finished
	char message[BUF] = {};
	char list = '1';
	char length[BUF] = {};

	int s = recv(create_socket, &length, BUF-1, 0);	// BUF-1 müsste ausreichen, da es nur eine Zahl sein sollte ;)
	if (s > 0) {
		length[s] = '\0';
		fileName.assign(length);
		std::cout << fileName << std::endl;
		std::string length = fileName.substr(0);
		//std::cout << "list: DEBUG: " << fileName << std::endl;
		len = atoi(length.c_str());
		//std::cout << "list: DEBUG: Laenge des Inhalts: " << len << std::endl;
	}

	send(this->create_socket, &list, 1, 0);

	s = recv(create_socket, &message, len, 0);
	if(s > 0) {
		message[s]= '\0';
		std::cout << message;
	}
	return;
}

void Socket::quit() {	// finished
	closeSocket();
}

/**
 * Use:
 * 		displaying error messages from server
 * Parameter: none
 */
void Socket::err() {	// finished
	char err = '5';
	//char _len[BUF];
	//recv(create_socket, &_len, BUF-1, 0);
	//len = atoi(_len);
	char message[len];
	char length[BUF] = {};

	int s = recv(create_socket, &length, BUF-1, 0);	// BUF-1 müsste ausreichen, da es nur eine Zahl sein sollte ;)
	if (s > 0) {
		length[s] = '\0';
		fileName.assign(length);
		//std::cout << "err: DEBUG: length = "fileName << std::endl;
		std::string length = fileName.substr(0);
		//std::cout << "err: DEBUG: " << fileName << std::endl;
		len = atoi(length.c_str());
		//std::cout << "err: DEBUG: Laenge des Inhalts: " << len << std::endl;
	}

	//std::cout << "err: DEBUG: len = " << len << std::endl;
	send(this->create_socket, &err, 1, 0);
	//std::cout << "err: DEBUG: send successful" << std::endl;
	int size = recv(create_socket, &message, len, 0); // ??
	//std::cout << "err: DEBUG: recv successful" << std::endl;
	if(size > 0) {
		//std::cout << "err: DEBUG: size = " << size << std::endl;
		message[size]= '\0';
		//std::cout << "err: DEBUG: nullterminieren funktioniert" << std::endl;
		std::cout << message;
	}
}

void Socket::login() {
	std::string user = "";
	std::string pass = "";
	std::string login = "";

	std::cout << "Please enter username: ";
	while(user.empty()) {
		getline(std::cin, user);
		if(user.empty()) std::cout << "Incorrect username!" << std::endl;
	}
	while(pass.empty()) {
		pass = getpass("Please enter password: ");
		if(pass.empty()) std::cout << "Incorrect password!" << std::endl;
	}

	login.assign("0 ");
	login.append(user);
	login.append(" ");
	login.append(pass);
	std::cout << "login: DEBUG: login-message: " << login << std::endl;

	send(this->create_socket, login.c_str(), login.length(), 0);

	recv(create_socket, &status, 1, 0); //get status info if login success

	if (status != '0') {
		std::cout << "login: DEBUG: status != 0; couldn't log in" << std::endl;
		err();
		return;
	}

	std::cout << "Sucessfully logged in! =)" << std::endl;
}
