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
protected:

public:
	ConnectionSocket(int socketID);
	virtual ~ConnectionSocket();

	bool send();
	bool sendFile();
	bool recv();
	bool recvFile();
};

}

#endif /* CONNECTIONSOCKET_H_ */
