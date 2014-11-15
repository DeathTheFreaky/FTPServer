/*
 * CommandHandler.cpp
 *
 *  Created on: 17.10.2014
 *      Author: clemens
 */

#include "CommandHandler.h"

CommandHandler::CommandHandler(ConnectionSocket *conn, Filemanager *mang) {
	this->conn = conn;
	this->mang = mang;
}

CommandHandler::~CommandHandler() {
	// TODO Auto-generated destructor stub
}

void CommandHandler::process(std::string *command){
	if(command->empty()){
		this->conn->stop();
		return;
	}
	std::string temp;
	temp = command->substr(0, command->find_first_of(' '));
	std::string errorcmd = temp;
	std::transform(temp.begin(), temp.end(), temp.begin(), toupper);
	if(temp.compare("LIST") == 0){
		list();
	}else if(temp.compare("GET") == 0){
		std::string file = command->substr(command->find_first_of(' ')+1);
		get(&file);
	}else if(temp.compare("PUT") == 0){
		std::string file = command->substr(command->find_first_of(' ')+1);
		put(&file);
	}else if(temp.compare("QUIT") == 0){
		quit();
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

void CommandHandler::list(){
	std::vector<std::string> *fileList;
	fileList = this->mang->getFilelist();
	std::string msg;
	msg.append("Files:\n");
	msg.append("------------------------------------\n");
	for(unsigned int i = 0; i < fileList->size(); i++){
		msg.append(fileList->at(i));
		msg.append("\n");
	}
	msg.append("------------------------------------\n");
// TODO padding?
	std::string init = "1 ";
	init.append(std::to_string(msg.length()));
	this->conn->sendData(&init);
	this->conn->recvData(&init);
	if(init.compare("1") != 0){
		std::cerr << "An unexpected list-response was received. (" << init <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}
	this->conn->sendData(&msg);
	delete fileList;
}

void CommandHandler::get(std::string *fileName){
	std::cout << *fileName << std::endl;
	File *file;
	if(fileName->find('\\') != std::string::npos){
		std::cout << "DEBUG: Tried to navigate out of the dir." << std::endl;
		std::string errormsg = "ERROR: A File is not allowed to contain a \"\\\"!\n";
		error(&errormsg);
		return;
	}
	if(!this->mang->fileExists(fileName)){
		std::cout << "DEBUG: File \"" << *fileName << "\" not found!" << std::endl;
		std::string errormsg = "ERROR: The file \"";
		errormsg.append(*fileName);
		errormsg.append("\" is not found or could not be opened!\n");
		error(&errormsg);
		return;
	}
	if((file = this->mang->getFile(fileName, true)) == NULL){
		delete file;
		std::cout << "DEBUG: File \"" << *fileName << "\" could not be opened!" << std::endl;
		std::string errormsg = "ERROR: The file \"";
		errormsg.append(*fileName);
		errormsg.append("\" is not found or could not be opened!\n");
		error(&errormsg);
		return;
	}

	std::string init = "2 ";
	init.append(std::to_string(file->getLength()));
	init.append(" ");
	init.append(*fileName);
	this->conn->sendData(&init);
	this->conn->recvData(&init);
	if(init.compare("2") != 0){
		std::cerr << "An unexpected get-response was received. (" << init <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}
	this->conn->sendData(file);
	std::cout << "GET finished." << std::endl;
}

void CommandHandler::put(std::string *fileName){
	File *file;
	if(fileName->find('\\') != std::string::npos){
		std::string errormsg = "ERROR: A File is not allowed to contain a \"\\\"!\n";
		error(&errormsg);
		return;
	}
	bool exists = this->mang->fileExists(fileName);
	if(exists){
		std::string errormsg = "ERROR: The File \"";
		errormsg.append(*fileName);
		errormsg.append("\" does already exist.\n");
		error(&errormsg);
		return;
	}

	if(!(file = this->mang->getFile(fileName, false))){
		delete file;
		std::string errormsg = "ERROR: Could not create the File \"";
		errormsg.append(*fileName);
		errormsg.append("\".\n");
		error(&errormsg);
		return;
	}
	std::string init = "3 ";
	init.append(*fileName);
	this->conn->sendData(&init);
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
	this->conn->sendData(&temp);
	this->conn->recvData(file, fileSize);
	delete file;
	std::cout << "PUT finished."  << std::endl;
}

void CommandHandler::quit(){
	std::string q = "4";
	this->conn->sendData(&q);
	this->conn->stop();
}

void CommandHandler::error(std::string *errormsg){
	std::string error = "5 ";
	error.append(std::to_string(errormsg->length()));
	std::cout << "DEBUG-CommandHandler-error: " << error << std::endl;
	std::string temp = "";
	this->conn->sendData(&error);
	this->conn->recvData(&temp);
	if(temp.compare("5") != 0){
		std::cerr << "An unexpected error-response was received. (" << temp <<")\n" << "Canceling the operation!" << std::endl;
		return;
	}
	this->conn->sendData(errormsg);
}
