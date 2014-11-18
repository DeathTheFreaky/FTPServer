/*
 * CommandHandler.cpp
 *
 *  Created on: 17.10.2014
 *      Author: clemens
 */

#include "CommandHandler.h"

/**
 * The CommandHandler is processing the commands sent by the client.
 * Parameters:
 * 		ConnectionSocket *conn: A pointer to the socket it is using for the communication
 * 		Filemanager *mang: A pointer to the filemanager to interact with Files in the download directory.
 * 		LDAPConnection *ldapcon: A pointer to the ldapconnection to authorize the users.
 */
CommandHandler::CommandHandler(ConnectionSocket *conn, Filemanager *mang, LDAPConnection *ldapcon) {
	this->conn = conn;
	this->mang = mang;
	this->ldapcon = ldapcon;
	this->loggedIn = false;
}

CommandHandler::~CommandHandler() {

}

/**
 * The public method processes the command which is passed to it.
 * Parameters:
 * 		std::string *command: The command to process.
 */
void CommandHandler::process(std::string *command){
	//if command is empty nothing is was recved from the client -> client closed connection
	if(command->empty()){
		this->conn->stop();
		return;
	}

	//spliting the string in command and parameters and transforming command to upper case
	std::string temp;
	temp = command->substr(0, command->find_first_of(' '));
	std::string errorcmd = temp;
	std::transform(temp.begin(), temp.end(), temp.begin(), toupper);

	//checking for the right command and if user is logged in
	if(temp.compare("LOGIN") == 0 && !this->loggedIn){
		login();
	}else if(temp.compare("LIST") == 0 && this->loggedIn){
		list();
	}else if(temp.compare("GET") == 0 && this->loggedIn){
		std::string file = command->substr(command->find_first_of(' ')+1);
		get(&file);
	}else if(temp.compare("PUT") == 0 && this->loggedIn){
		std::string file = command->substr(command->find_first_of(' ')+1);
		put(&file);
	}else if(temp.compare("QUIT") == 0){
		quit();
	}else if(!this->loggedIn){
		std::string errormsg = "You have to \"LOGIN\" first to get access to other commands then \"QUIT\".\n";
		error(&errormsg);
	}else{
		std::string errormsg = "The command \"";
		errormsg.append(errorcmd);
		errormsg.append("\" is not an valid command.");
		errormsg.append("The following commands are valid:\n");
		errormsg.append("LIST\n");
		errormsg.append("GET <filename>\n");
		errormsg.append("PUT <filename>\n");
		errormsg.append("QUIT\n");
		errormsg.append("------------------------------------\n");
		error(&errormsg);
	}
}

/**
 * The private method login tries to log the user with the provided data in.
 */
void CommandHandler::login(){
	//checks if user is allowed to log in
	if(!this->conn->checkIP()){
		std::string errormsg = "ERROR: You tried to login to often at a time. You have to wait ";
		errormsg.append(std::to_string(this->conn->getWaitTime()));
		errormsg.append("s until you can try again.\n");
		error(&errormsg);
		return;
	}

	//sends client start acknowledge for login procedure
	std::string init = "0";
	this->conn->sendData(&init);

	//recvs the login data and gets status code from client
	this->conn->recvData(&init);
	std::string temp = init.substr(0, init.find_first_of(' '));
	init = init.substr(init.find_first_of(' ')+1);
	if(temp.compare("0") != 0){
		std::cerr << "An unexpected login-response was received. (" << init <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}

	//adds a login attempt for the users IP-address and prepares the userdata
	this->conn->addLoginAttempt();
	temp = init.substr(0, init.find_first_of(' '));
	init = init.substr(init.find_first_of(' ')+1);

	//tries to authenticate user if successfull send success acknowledge back and reset IP
	//if not send error
	if(this->ldapcon->auth(&temp, &init)){
		init.assign("0");
		this->conn->sendData(&init);
		this->conn->resetIP();
		this->loggedIn = true;
	}else{
		std::string errormsg = "ERROR: The entered username and password are invalid!\n";
		error(&errormsg);
	}
	return;
}

/**
 * The private method list returns a list of all files in the download directory
 */
void CommandHandler::list(){
	//gets a list of all files
	std::vector<std::string> *fileList;
	fileList = this->mang->getFilelist();

	//prepares the msg for sending
	std::string msg;
	msg.append("Files:\n");
	msg.append("------------------------------------\n");
	for(unsigned int i = 0; i < fileList->size(); i++){
		msg.append(fileList->at(i));
		msg.append("\n");
	}
	msg.append("------------------------------------\n");

	//sends acknowledge to client
	std::string init = "1 ";
	init.append(std::to_string(msg.length()));
	this->conn->sendData(&init);

	//recvs rdy acknowledge
	this->conn->recvData(&init);
	if(init.compare("1") != 0){
		std::cerr << "An unexpected list-response was received. (" << init <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}

	//sends list
	this->conn->sendData(&msg);
	delete fileList;
}

/**
 * The private method get sends the requested file to the client (if it exists)
 */
void CommandHandler::get(std::string *fileName){
	File *file;

	/*check if file path does not contain a*/
	if(fileName->find('\\') != std::string::npos){
		std::cout << "DEBUG: Tried to navigate out of the dir." << std::endl;
		std::string errormsg = "ERROR: A File is not allowed to contain a \"\\\"!\n";
		error(&errormsg);
		return;
	}

	//check if file exists
	if(!this->mang->fileExists(fileName)){
		std::cout << "DEBUG: File \"" << *fileName << "\" not found!" << std::endl;
		std::string errormsg = "ERROR: The file \"";
		errormsg.append(*fileName);
		errormsg.append("\" is not found or could not be opened!\n");
		error(&errormsg);
		return;
	}

	//try to open the file
	if((file = this->mang->getFile(fileName, true)) == NULL){
		delete file;
		std::cout << "DEBUG: File \"" << *fileName << "\" could not be opened!" << std::endl;
		std::string errormsg = "ERROR: The file \"";
		errormsg.append(*fileName);
		errormsg.append("\" is not found or could not be opened!\n");
		error(&errormsg);
		return;
	}

	//send acknowledge to client
	std::string init = "2 ";
	init.append(std::to_string(file->getLength()));
	init.append(" ");
	init.append(*fileName);
	this->conn->sendData(&init);

	//recv acknowledge from client
	this->conn->recvData(&init);
	if(init.compare("2") != 0){
		std::cerr << "An unexpected get-response was received. (" << init <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}

	//send the file
	this->conn->sendData(file);
}

/**
 * The private method put recvs a file and safes it in the file system.
 */
void CommandHandler::put(std::string *fileName){
	File *file;
	/*Check if file path does not contain a \*/
	if(fileName->find('\\') != std::string::npos){
		std::string errormsg = "ERROR: A File is not allowed to contain a \"\\\"!\n";
		error(&errormsg);
		return;
	}

	//check if file already exists
	if(this->mang->fileExists(fileName)){
		std::string errormsg = "ERROR: The File \"";
		errormsg.append(*fileName);
		errormsg.append("\" does already exist.\n");
		error(&errormsg);
		return;
	}

	//try to open the file
	if(!(file = this->mang->getFile(fileName, false))){
		delete file;
		std::string errormsg = "ERROR: Could not create the File \"";
		errormsg.append(*fileName);
		errormsg.append("\".\n");
		error(&errormsg);
		return;
	}

	//send acknowledge to client
	std::string init = "3 ";
	init.append(*fileName);
	this->conn->sendData(&init);

	//recv acknowledge and size from client
	this->conn->recvData(&init);
	std::string temp = init.substr(0, init.find_first_of(' '));
	init = init.substr(init.find_first_of(' ')+1);
	if(temp.compare("3") != 0){
		std::cerr << "An unexpected put-response was received. (" << init <<")\n" << "Canceling the operation!" << std::endl;
		file->close();
		delete file;
		this->mang->deleteFile(fileName);
		return;
	}
	long fileSize = std::stol(init);

	//send acknowledge to client
	this->conn->sendData(&temp);

	//recv file
	this->conn->recvData(file, fileSize);
	delete file;
}

/**
 * The private method quit closes the connection to the client
 */
void CommandHandler::quit(){
	//send acknowledge to client and close the connection
	std::string q = "4";
	this->conn->sendData(&q);
	this->conn->stop();
}

/**
 * The private method error sends an error string to the client
 */
void CommandHandler::error(std::string *errormsg){
	//send acknowledge to client
	std::string error = "5 ";
	error.append(std::to_string(errormsg->length()));
	std::string temp = "";
	this->conn->sendData(&error);

	//recv acknowledge from client
	this->conn->recvData(&temp);
	if(temp.compare("5") != 0){
		std::cerr << "An unexpected error-response was received. (" << temp <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}

	//send error message
	this->conn->sendData(errormsg);
}
