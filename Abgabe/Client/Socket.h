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
#include <cstdlib>

#ifndef SOCKET_H_
#define SOCKET_H_

#define BUF 1024

class Socket {
public:
	Socket(std::string ip, int port);
	virtual ~Socket();

	bool conn();
	void closeSocket();
	void sendCommand(std::string comm);
	void receiveAnswer();
	void getData(int len);
	void putData();
	void showList();
	void quit();
	int getStatus();
	int getLength();
	void err();

private:
	int create_socket;
	int port;
	std::string ip;
	struct sockaddr_in address;
	int size;
	int status;
	int len;
};

#endif /* SOCKET_H_ */
