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
#include "Server.h"
using namespace std;

void printUsage(char *programName);

int main(int argc, char *argv[]) {
	string baseDir;
	int port;
	string _port;
	stringstream convert;
	Server::Server *server;

	//check if the required parameters are passed
	if(argc != 3){
		printUsage(argv[0]);
		exit(1);
	}

	//check if port is a number
	_port = (string)argv[1];
	convert << _port;
	if(!(convert >> port)){
		cerr << "Error: The port must be a number." << endl;
		printUsage(argv[0]);
		exit(2);
	}

	//check if port is between 1024 and 65535
	if(!(port >= 1024 && port <= 65535)){
		cerr << "Error: The port must be between 1024 and 65535." << endl;
		printUsage(argv[0]);
		exit(3);
	}

	baseDir = (string)argv[2];

	cout << "port = " << port << "; dir = " << baseDir << endl;

	server = new Server::Server(port, baseDir);
	server->start();

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
