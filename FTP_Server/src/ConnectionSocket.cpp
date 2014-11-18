/*
 * ConnectionSocket.cpp
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#include "ConnectionSocket.h"
#define BUF 1024

/**
 * The ConnectionSocket is managing the whole communication between server and client.
 * Parameter:
 * 		int socketID: ID of the newly opened socket for the communication
 * 		Filemanager *filemanager: reference to a filemanager to grant access to the files in the download directory
 * 		LDAPConnection *ldapcon: reference to a ldap connection to grant access to the authentication mechanismus
 * 		std::string *currentIP: reference to a string which contains the current IP
 * 		std::map<std::string, std::array<long,2>> *ips: reference to an map wich is used for logging the login attempts
 */
ConnectionSocket::ConnectionSocket(int socketID, Filemanager *filemanager,
		LDAPConnection *ldapcon, std::string *currentIP,
		std::map<std::string, std::array<long,2>> *ips) {
	this->socketID = socketID;
	this->cmd = new CommandHandler(this, filemanager, ldapcon);
	this->work = true;
	this->ips = ips;
	this->currentIP = currentIP;
}

/**
 * Closing/deleting everything that is unique to this connection
 */
ConnectionSocket::~ConnectionSocket() {
	close(this->socketID);
	std::cout << "Connection to " << *this->currentIP << " closed." << std::endl;
	delete this->cmd;
	delete this->currentIP;
}

/**
 * The public method start is called to start the connection (by thread)
 */
void ConnectionSocket::start() {
	//creates command string and sends welcome message
	std::string *command = new std::string;
	this->welcome();

	//processes all recved commands from the client with the command handler
	do {
		recvData(command);
		this->cmd->process(command);
		command->clear();
	} while (work);

	//closes the connection
	delete command;
	delete this;
}

/**
 * The public method stop is used to stop the connection between the Server and the Client
 */
void ConnectionSocket::stop() {
	this->work = false;
}

/**
 * The private method welcome sends the welcome message to the client
 */
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

/**
 * The public method checkIP checks if the Client is allowed to have another login attempt
 * Return:
 * 		bool: true if client is allowed to hav another login attempt, false if not
 */
bool ConnectionSocket::checkIP(){
	//gets an iterator to the object of the clients IP
	auto it = this->ips->find(*this->currentIP);

	//if none is found it creats a new one
	if(it == this->ips->end()){
		this->ips->insert(std::pair<std::string, std::array<long,2>>(*this->currentIP, {0,time(NULL)}));
		return true;
	}

	//if on the IP are less then 3 attempts logged
	if(it->second[0] < 3){
		return true;
	}

	//if the last attempt wasnt withing the last TIMEOUT seconds
	if((it->second[1] + TIMEOUT) < time(NULL)){
		it->second[0] = 0;
		it->second[1] = time(NULL);
		return true;
	}
	return false;
}

/**
 * The public method addLoginAttempt adds an login attempt to the clients IP
 */
void ConnectionSocket::addLoginAttempt(){
	//gets interator for clients IP, adds an attempt to it and sets the current time
	auto it = this->ips->find(*this->currentIP);
	it->second[0]++;
	it->second[1] = time(NULL);
}

/**
 * The public method resetIP resets the attempts from an IP
 */
void ConnectionSocket::resetIP(){
	auto it = this->ips->find(*this->currentIP);
	it->second[0] = 0;
}

/**
 * The public method getWaitTime returns the time in seconds the client has to wait
 * until another login attempt can be performed
 * Return:
 * 		long: time in seconds until another login attempt can be performed
 */
long ConnectionSocket::getWaitTime(){
	auto it = this->ips->find(*this->currentIP);
	return (time(NULL)-it->second[1]);
}

/**
 * The public method recvData recvs maximal the next BUF bytes from the stream and
 * writes them into the std::string passed to it
 * Parameters:
 * 		std::string *cmd: The recved bytes are written into it
 */
void ConnectionSocket::recvData(std::string *cmd) {
	//prepares everything for recv
	int size = 0;
	char buffer[BUF];
	cmd->clear();

	//recvs from socket and acts on base of the result
	size = recv(this->socketID, &buffer, BUF - 1, 0);
	buffer[size] = '\0';
	if (size > 0) {
		cmd->append(buffer);
	} else if (size == 0) {
		printf("Client closed remote socket\n");
		this->stop();
	} else {
		perror("recv error");
		exit(1);
	}
}

/**
 * The public method recvData recvs a file with fileSize
 * Parameters:
 * 		File *file: a reverence to the file where it should be saved
 * 		long fileSize: file size of the recving file
 */
void ConnectionSocket::recvData(File *file, long fileSize) {
	int size = 0, recved = 0;
	char buffer[BUF];

	//recvs bytes untile everything is recved
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

/**
 * The public method sendData sends the passed string.
 * Parameters:
 * 		std::string *msg: the message that should be sent
 */
void ConnectionSocket::sendData(std::string *msg) {
	int sended = 0, s = 0;
	int msglength = msg->length();

	//send msg until everything is sent
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


/**
 * The public method send Data sends the passed file
 * Parameters:
 * 		File *file: a reverence to the file which should be sent
 */
void ConnectionSocket::sendData(File *file) {
	unsigned long sended = 0, length = file->getLength();
	int s = 0;

	char buffer[BUF];
	while (length > sended) {
		// read data as a block
		file->readNextBytes(buffer, BUF);

		//send block
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
	}
}
