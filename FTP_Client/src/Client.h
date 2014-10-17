/*
 * Client.h
 *
 *  Created on: 10.10.2014
 *      Author: Alexander Benesch
 */

#include <string>

#include "Socket.h"
using namespace std;

#ifndef CLIENT_H_
#define CLIENT_H_

class Client {
public:
	Client(std::string ip, int port);
	virtual ~Client();
	int clientStart();
private:
	std::string ipAdress;
	int port;
};

#endif /* CLIENT_H_ */
