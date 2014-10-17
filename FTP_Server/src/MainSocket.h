/*
 * Socket.h
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#ifndef MAINSOCKET_H_
#define MAINSOCKET_H_

namespace Server{

class MainSocket {
private:
	int port;
protected:
public:
	MainSocket(int port);
	virtual ~MainSocket();

	int accept();
};

}

#endif /* MAINSOCKET_H_ */
