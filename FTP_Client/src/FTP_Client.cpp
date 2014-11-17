//============================================================================
// Name        : FTP_Client.cpp
// Author      : Alexander Benesch, Clemens Grabmann
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <signal.h>

#include "Client.h"

using namespace std;

Client *client = NULL;

void printUsage(char *programName);
void signal_handler(int sig);

int main(int argc, char *argv[]) {
	string ip;
	int port;
	string _port;

	//registering signal handler
	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		std::cout << "Cannot handle SIGINT!" << std::endl;
		return 1;
	}
	if (signal(SIGTERM, signal_handler) == SIG_ERR) {
		std::cout << "Cannot handle SIGTERM!" << std::endl;
		return 1;
	}

	// checks if required parameters are passed
	if (argc != 3) {
		printUsage(argv[0]);
		return 1;
	}

	// check if port is a number + convert to int
	_port = (string)(argv[2]);
	stringstream convert;
	convert << _port;
	if (!(convert >> port)) {
		cout << "Error: Port is not a number" << endl;
		return 1;
	}

	// check if port is valid
	if (!(port >= 1024 && port <= 65535)) {
		cout << "Error: Port is not valid" << endl;
		return 1;
	}

	ip = argv[1];

	client = new Client(ip, port);

	client->clientStart();

	return 0;
}

/**
 * Use:
 * 		The method prints the usage message for the program.
 *
 * Parameter:
 * 		char *programName: points to an array containing the program name
 */
void printUsage(char *programName) {
	cout << "Usage: " << programName << " <IP-ADRESS> <PORT>" << endl;
}

/**
 * Use:
 * 		Signal handler to intercept SIGINT and SIGTERM signals
 *
 * Parameter:
 * 		int sig: number of signal
 */
void signal_handler(int sig) {
	if(sig == SIGINT) {
		delete client;
		exit(0);
	}
}
