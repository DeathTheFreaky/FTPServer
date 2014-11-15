/*
 * ConnectionSocket.h
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#ifndef CONNECTIONSOCKET_H_
#define CONNECTIONSOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "CommandHandler.h"
#include "Filemanager.h"
#include "File.h"
#include "LDAPConnection.h"

class CommandHandler;

class ConnectionSocket {
private:
	int socketID;
	bool work;
	CommandHandler *cmd;

	void welcome();
protected:

public:
	ConnectionSocket(int socketID, Filemanager *filemanager, LDAPConnection *ldapcon);
	virtual ~ConnectionSocket();

	void start();
	void stop();

	void sendData(std::string *msg);
	void sendData(File *file);
	void recvData(std::string *cmd);
	void recvData(File *file, long fileSize);
};


#endif /* CONNECTIONSOCKET_H_ */
