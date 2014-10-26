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
#include <unistd.h>
#include <iostream>
#include "CommandHandler.h"
#include "Filemanager.h"
#include "File.h"

class CommandHandler;

class ConnectionSocket {
private:
	int socketID;
	bool work;
	Filemanager *filemanager;
	CommandHandler *cmd;

	void welcome();
protected:

public:
	ConnectionSocket(int socketID, Filemanager *filemanager);
	virtual ~ConnectionSocket();

	void start();
	void stop();

	void sendData(std::string *msg);
	void sendData(File *file);
	void recvData(std::string *cmd);
};


#endif /* CONNECTIONSOCKET_H_ */
