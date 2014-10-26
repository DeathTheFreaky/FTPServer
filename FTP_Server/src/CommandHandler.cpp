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
	std::string temp;
	temp = command->substr(0, command->find_first_of(' '));
	std::string errorcmd = temp;
	std::transform(temp.begin(), temp.end(), temp.begin(), toupper);
	if(temp.compare("LIST") == 0){
		list();
	}else if(temp.compare("GET") == 0){

	}else if(temp.compare("PUT") == 0){

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

void CommandHandler::get(){

}

void CommandHandler::put(){

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
