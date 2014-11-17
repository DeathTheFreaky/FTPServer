/*
 * Server.h
 *
 *  Created on: 09.10.2014
 *      Author: Clemens Grabmann
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <thread>
#include <vector>
#include <map>
#include "Filemanager.h"
#include "MainSocket.h"
#include "ConnectionSocket.h"
#include "LDAPConnection.h"
#include "Threadmanager.h"

class Server {
private:
	MainSocket *mainSocket;
	Filemanager *filemanager;
	LDAPConnection *ldapcon;
	Threadmanager *threadmang;
	std::map<std::string, std::array<long,2>> *ips;
	std::vector<ConnectionSocket*> *connections;
	std::vector<std::thread> *threadList;
	std::thread t;
protected:
public:
	Server(int port, std::string *dir);
	virtual ~Server();

	void start();
};


#endif /* SERVER_H_ */
