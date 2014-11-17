/*
 * ConnectionSocket.cpp
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#include "ConnectionSocket.h"
#define BUF 1024

ConnectionSocket::ConnectionSocket(int socketID, Filemanager *filemanager,
		LDAPConnection *ldapcon, std::string *currentIP,
		std::map<std::string, std::array<long,2>> *ips) {
	this->socketID = socketID;
	this->cmd = new CommandHandler(this, filemanager, ldapcon);
	this->work = true;
	this->ips = ips;
	this->currentIP = currentIP;
}

ConnectionSocket::~ConnectionSocket() {
	delete this->cmd;
	delete this->currentIP;
	close(this->socketID);
}

void ConnectionSocket::start() {
	std::string *command = new std::string;
	this->welcome();
	do {
		recvData(command);
		cmd->process(command);
		command->clear();
	} while (work);
	delete command;
	delete this;
}

void ConnectionSocket::stop() {
	this->work = false;
}

void ConnectionSocket::welcome() {
	std::string wel;
	wel.append("Welcome on the Server.\n");
	wel.append("The following commands are valid:\n");
	wel.append("LOGIN\n");
	wel.append("LIST\n");
	wel.append("GET <filename>\n");
	wel.append("PUT <filename>\n");
	wel.append("QUIT\n");
	wel.append("------------------------------------\n");
	wel.resize(BUF, '\0');
	sendData(&wel);
}

bool ConnectionSocket::checkIP(){
	auto it = this->ips->find(*this->currentIP);
	if(it == this->ips->end()){
		//this->ips[*this->currentIP] = std::array<long,2>{0,time(NULL)};
		this->ips->insert(std::pair<std::string, std::array<long,2>>(*this->currentIP, {0,time(NULL)}));
		return true;
	}
	if(it->second[0] < 3){
		return true;
	}
	if((it->second[1] + TIMEOUT) < time(NULL)){
		it->second[0] = 0;
		it->second[1] = time(NULL);
		return true;
	}
	return false;
}

void ConnectionSocket::addLoginAttempt(){
	auto it = this->ips->find(*this->currentIP);
	it->second[0]++;
	it->second[1] = time(NULL);
}

void ConnectionSocket::resetIP(){
	auto it = this->ips->find(*this->currentIP);
	it->second[0] = 0;
}

long ConnectionSocket::getWaitTime(){
	auto it = this->ips->find(*this->currentIP);
	return (time(NULL)-it->second[1]);
}

void ConnectionSocket::recvData(std::string *cmd) {
	int size = 0;
	char buffer[BUF];
	cmd->clear();
	size = recv(this->socketID, &buffer, BUF - 1, 0);
	buffer[size] = '\0';
	if (size > 0) {
		cmd->append(buffer);
	} else if (size == 0) {
		printf("Client closed remote socket\n");
	} else {
		perror("recv error");
		exit(1);
	}
}

void ConnectionSocket::recvData(File *file, long fileSize) {
	int size = 0, recved = 0;
	char buffer[BUF];
	while (fileSize > recved) {
		size = recv(this->socketID, buffer, BUF, 0);
		if (size > 0) {
			file->writeNextBytes(buffer, size);
			recved += size;
		} else if (size == 0) {
			printf("Client closed remote socket\n");
			this->stop();
			return;
		} else {
			perror("recv error");
			file->close();
			delete file;
			exit(1);
		}
	}
}

void ConnectionSocket::sendData(std::string *msg) {
	int sended = 0, s = 0;
	int msglength = msg->length();
	while (msglength > sended) {
		s = send(this->socketID, &msg->c_str()[sended], (msglength - sended),
				MSG_NOSIGNAL);
		if (s != -1) {
			sended += s;
		} else {
			break;
		}
	}
}

void ConnectionSocket::sendData(File *file) {
	unsigned long sended = 0, length = file->getLength();
	int s = 0;

	char buffer[BUF];
	while (length > sended) {
		// read data as a block:
		file->readNextBytes(buffer, BUF);

		if ((length - sended) >= BUF) {
			s = send(this->socketID, buffer, BUF, MSG_NOSIGNAL);
		} else {
			s = send(this->socketID, buffer, length - sended, MSG_NOSIGNAL);
		}
		if (s != -1) {
			sended += s;
		} else {
			break;
		}
		//std::cout << "msglength: " << file->getLength() << "; sended: " << sended << " send returned: " << s << "; error: " << errno << std::endl;
	}
}
