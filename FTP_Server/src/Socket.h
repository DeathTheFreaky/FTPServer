/*
 * Socket.h
 *
 *  Created on: 15.10.2014
 *      Author: clemens
 */

#ifndef SOCKET_H_
#define SOCKET_H_

namespace Server{

class Socket {
private:
	int port;
protected:

public:
	Socket(int port);
	virtual ~Socket();
};

} //namespace end

#endif /* SOCKET_H_ */
