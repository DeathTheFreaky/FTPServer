/*
 * ConnectionSocket.h
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#ifndef CONNECTIONSOCKET_H_
#define CONNECTIONSOCKET_H_

namespace Server {

#include "Filemanager.h"
#include "File.h"

class ConnectionSocket {
private:
	int socketID;
	Filemanager *filemanager;

	bool send(std::string *msg);
	bool send(Server::File *file);
	bool recv();
protected:

public:
	ConnectionSocket(int socketID, Filemanager *filemanager);
	virtual ~ConnectionSocket();

	void start();
};

}

#endif /* CONNECTIONSOCKET_H_ */
