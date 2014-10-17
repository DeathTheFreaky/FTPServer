/*
 * ConnectionSocket.h
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#ifndef CONNECTIONSOCKET_H_
#define CONNECTIONSOCKET_H_

namespace Server {

class ConnectionSocket {
private:
	int socketID;
	Filemanager *filemanager;
protected:

public:
	ConnectionSocket(int socketID, Filemanager *filemanager);
	virtual ~ConnectionSocket();

	bool send(std::string *msg);
	bool sendFile(File *file);
	bool recv();
	bool recvFile();
};

}

#endif /* CONNECTIONSOCKET_H_ */
