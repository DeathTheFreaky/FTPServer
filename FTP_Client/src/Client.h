/*
 * Client.h
 *
 *  Created on: 10.10.2014
 *      Author: Alexander Benesch
 */

#include <string>
#include <locale>

#include "Socket.h"
using namespace std;

#ifndef CLIENT_H_
#define CLIENT_H_

class Client {
public:
	Client(std::string ip, int port);
	virtual ~Client();
	void clientStart();
private:
	std::string ipAdress;
	int port;
	std::string toUpper(std::string);
	bool quit;
	Socket *socket;
};

#endif /* CLIENT_H_ */
