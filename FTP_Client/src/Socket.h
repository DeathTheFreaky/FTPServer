/*
 * Socket.h
 *
 *  Created on: 17.10.2014
 *      Author: Alexander Benesch
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <cstring>

#ifndef SOCKET_H_
#define SOCKET_H_

class Socket {
public:
	Socket(std::string ip, int port);
	virtual ~Socket();

	bool conn();
	void closeSocket();
	void sendCommand(std::string comm);
	int getData();
	int putData();

private:
	int create_socket;
	int port;
	std::string ip;
	struct sockaddr_in address;
	std::string buffer;
	int size;
};

#endif /* SOCKET_H_ */
