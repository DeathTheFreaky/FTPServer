/*
 * Threadmanager.h
 *
 *  Created on: 17.11.2014
 *      Author: clemens
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include <thread>
#include <vector>
#include <iostream>
#include "ConnectionSocket.h"

class Threadmanager {
private:
	std::vector<std::thread> *threadList;
	std::vector<ConnectionSocket*> *connections;
	bool work;
public:
	Threadmanager(std::vector<std::thread> *threadList, std::vector<ConnectionSocket*> *connections);
	virtual ~Threadmanager();

	void manage();
	void stop();
};

#endif /* THREADMANAGER_H_ */
