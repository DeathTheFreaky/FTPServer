/*
 * Server.cpp
 *
 *  Created on: 09.10.2014
 *      Author: Clemens Grabmann
 */

#include "Server.h"

/**
 * Parameter:
 * 		port: The port the server should listen at
 * 		dir: The pointer to the string where the path to the used directory is saved
 */
Server::Server(int port, std::string *dir) {
	this->mainSocket = new MainSocket(port);
	this->filemanager = new Filemanager(dir);
	this->ldapcon = new LDAPConnection();
	this->ips = new std::map<std::string, std::array<long, 2>>;
	this->threadList = new std::vector<std::thread>();
	this->connections = new std::vector<ConnectionSocket*>();
	this->threadmang = new Threadmanager(this->threadList, this->connections);
	this->t = std::thread(&Threadmanager::manage, this->threadmang);
}

Server::~Server() {
	this->threadmang->stop();
	this->t.join();
	delete this->threadmang;
	delete this->threadList;
	delete this->mainSocket;
	delete this->filemanager;
	delete this->ldapcon;
	delete this->ips;
}

/**
 * This method starts the server.
 * The server starts to accept incoming clients and processes them.
 */
void Server::start() {
	ConnectionSocket *conn;
	struct sockaddr_in cliaddress;
	while (true) {
		std::cout << "waiting for connection" << std::endl;
		conn = new ConnectionSocket(this->mainSocket->sAccept(&cliaddress),
				this->filemanager, this->ldapcon,
				new std::string(inet_ntoa(cliaddress.sin_addr)), this->ips);
		this->connections->push_back(conn);
		threadList->push_back(std::thread(&ConnectionSocket::start, conn));
		/*conn = new ConnectionSocket(this->mainSocket->sAccept(&cliaddress),
		 this->filemanager, this->ldapcon,
		 new std::string(inet_ntoa(cliaddress.sin_addr)), this->ips);*/
		std::cout << "accepted connection" << std::endl;
		//conn->start();
	}
}

