/*
 * Socket.h
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#ifndef MAINSOCKET_H_
#define MAINSOCKET_H_

#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

class MainSocket {
private:
	int s;
	socklen_t addrlen;
	struct sockaddr_in address;
	int port;
protected:
public:
	MainSocket(int port);
	virtual ~MainSocket();

	int sAccept(struct sockaddr_in **cliaddress);
};


#endif /* MAINSOCKET_H_ */
