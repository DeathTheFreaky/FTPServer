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
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstdio>

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
	void getData();
	void putData();
	void showList();
	void quit();
	int getStatus();
	int getLength();
	void err();
	void login();

private:
	int create_socket;
	int port;
	std::string ip;
	struct sockaddr_in address;
	int size;
	int status;
	int len;
	std::string fileName;
};

#endif /* SOCKET_H_ */
