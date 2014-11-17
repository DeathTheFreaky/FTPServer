/*
 * Threadmanager.cpp
 *
 *  Created on: 17.11.2014
 *      Author: clemens
 */

#include "Threadmanager.h"

Threadmanager::Threadmanager(std::vector<std::thread> *threadList, std::vector<ConnectionSocket*> *connections) {
	this->threadList = threadList;
	this->connections = connections;
	this->work = true;
}

Threadmanager::~Threadmanager() {
	std::cout << "Closing all connections." << std::endl;
	for(unsigned int i = 0; i < this->connections->size(); i++){
		(*this->connections)[i]->serverStop();
	}
	for(unsigned int i = 0; i < this->threadList->size(); i++){
		std::cout << "Closing thread with ID " << (*this->threadList)[i].get_id() << "." << std::endl;
		(*this->threadList)[i].join();
		this->threadList->erase(this->threadList->begin()+i);
		std::cout << "Successful." << std::endl;
	}
	std::cout << "All connection closed." << std::endl;
}

void Threadmanager::manage(){
	while(this->work){
		for(unsigned int i = 0; i < this->threadList->size(); i++){
			if((*this->threadList)[i].joinable()){
				std::cout << "Closing thread with ID " << (*this->threadList)[i].get_id() << "." << std::endl;
				(*this->threadList)[i].join();
				this->threadList->erase(this->threadList->begin()+i);
				std::cout << "Successful." << std::endl;
				delete (*this->connections)[i];
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
}

void Threadmanager::stop(){
	this->work = false;
}
