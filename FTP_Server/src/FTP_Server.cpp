//============================================================================
// Name        : FTP_Server.cpp
// Author      : Alexander Benesch, Clemens Grabmann
// Version     :
// Copyright   :
// Description : A simple FTP-Server, VSYS exercise
//============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <csignal>
#include "Server.h"
using namespace std;

void printUsage(char *programName);
void signalHandler(int sig);

Server *server = NULL;
string *baseDir;

int main(int argc, char *argv[]) {
	int port;
	string _port;
	stringstream convert;

	if(signal(SIGINT, signalHandler) == SIG_ERR){
		std::cout << "Unable to bind signalHandler. Stopping Server." << std::endl;
		exit(1);
	}

	if(signal(SIGTERM, signalHandler) == SIG_ERR){
		std::cout << "Unable to bind signalHandler. Stopping Server." << std::endl;
		exit(1);
	}

	//check if the required parameters are passed
	if(argc != 3){
		printUsage(argv[0]);
		exit(2);
	}

	//check if port is a number
	_port = (string)argv[1];
	convert << _port;
	if(!(convert >> port)){
		cerr << "Error: The port must be a number." << endl;
		printUsage(argv[0]);
		exit(3);
	}

	//check if port is between 1024 and 65535
	if(!(port >= 1024 && port <= 65535)){
		cerr << "Error: The port must be between 1024 and 65535." << endl;
		printUsage(argv[0]);
		exit(4);
	}

	baseDir = new string;
	baseDir->assign(argv[2]);

	cout << "port = " << port << "; dir = " << *baseDir << endl;

	server = new Server(port, baseDir);
	server->start();

	delete baseDir;
	delete server;

	return 0;
}

/**
 * The method prints the usage message for the program.
 * Parameter:
 * 		char *programName: points to an array with the program name
 */
void printUsage(char *programName){
	cerr << "Usage: " << programName << " <PORT> <DIR>" << endl;
}

void signalHandler(int sig){
	std::cout << std::endl << "Server is shutting down." << std::endl;
	if(server != NULL){
		delete server;
		delete baseDir;
	}
	std::cout << "Server successfully shut down." << std::endl;
	exit(0);
}
