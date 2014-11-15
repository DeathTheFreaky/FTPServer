/*
 * CommandHandler.h
 *
 *  Created on: 17.10.2014
 *      Author: clemens
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include "ConnectionSocket.h"
#include "Filemanager.h"

class ConnectionSocket;

class CommandHandler {
private:
	ConnectionSocket *conn;
	Filemanager *mang;

	void error(std::string *command);
	void list();
	void get(std::string *file);
	void put(std::string *file);
	void quit();
protected:

public:
	CommandHandler(ConnectionSocket *conn, Filemanager *mang);
	virtual ~CommandHandler();

	void process(std::string *errorcmd);
};


#endif /* COMMANDHANDLER_H_ */
