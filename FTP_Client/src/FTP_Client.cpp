//============================================================================
// Name        : FTP_Client.cpp
// Author      : Clemens, Benesch
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

void printUsage(char *programName);

int main(int argc, char *argv[]) {
	string ip;
	int port;
	string _port;

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

	// debug message
	//cout << "IP Adress: " << ip << endl << "Port: " << port << endl;



	return 0;
}

/**
 * The method prints the usage message for the program.
 * Parameter:
 * 		char *programName: points to an array containing the program name
 */
void printUsage(char *programName) {
	cout << "Usage: " << programName << " <IP-ADRESS> <PORT>" << endl;
}
