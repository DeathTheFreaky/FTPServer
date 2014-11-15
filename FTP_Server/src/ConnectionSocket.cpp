/*
 * ConnectionSocket.cpp
 *
 *  Created on: 16.10.2014
 *      Author: clemens
 */

#include "ConnectionSocket.h"
#define BUF 1024

ConnectionSocket::ConnectionSocket(int socketID, Filemanager *filemanager) {
	this->socketID = socketID;
	this->filemanager = filemanager;
	this->cmd = new CommandHandler(this, this->filemanager);
	this->work = true;
}

ConnectionSocket::~ConnectionSocket() {
	delete this->cmd;
	close(this->socketID);
}

void ConnectionSocket::start(){
	std::string *command = new std::string;
	this->welcome();
	do{
		recvData(command);
		cmd->process(command);
		command->clear();
	}while(work);
	delete command;
	delete this;
}

void ConnectionSocket::stop(){
	this->work = false;
}

void ConnectionSocket::welcome(){
	std::string wel;
	wel.append("Welcome on the Server.\n");
	wel.append("The following commands are valid:\n");
	wel.append("LIST\n");
	wel.append("GET <filename>\n");
	wel.append("PUT <filename>\n");
	wel.append("QUIT\n");
	wel.append("------------------------------------\n");
	wel.resize(BUF,'\0');
	sendData(&wel);
}

void ConnectionSocket::recvData(std::string *cmd){
	int size = 0;
	char buffer[BUF];
	cmd->clear();
	size = recv(this->socketID, &buffer, BUF-1, 0);
	buffer[size] = '\0';
	std::cout << "recv=" << buffer << "; size=" << size << std::endl;
	if( size > 0){
		cmd->append(buffer);
	}else if (size == 0){
	   printf("Client closed remote socket\n");
	}else{
	   perror("recv error");
	   exit(1);
	}
}

void ConnectionSocket::recvData(std::fstream *file, long fileSize){
	int size = 0, recved = 0;
	char buffer[BUF];
	while(fileSize > recved){
		size = recv(this->socketID, &buffer, BUF, 0);
		if( size > 0){
			file->write(buffer, size);
			recved += size;
			std::cout << "recved=" << recved << "; filesize=" << fileSize << std::endl;
		}else if (size == 0){
		   printf("Client closed remote socket\n");
		   this->stop();
		   return;
		}else{
		   perror("recv error");
		   file->close();
		   delete file;
		   exit(1);
		}
	}
}

void ConnectionSocket::sendData(std::string *msg){
	int sended = 0, s = 0;
	int msglength = msg->length();
	while(msglength > sended){
		s = send(this->socketID, &msg->c_str()[sended], (msglength - sended), 0);
		if(s != -1){
			sended += s;
		}else{
			break;
		}
		std::cout << "msglength: " << msglength << "; sended: " << sended << " send returned: " << s << "; error: " << errno << std::endl;
	}
	std::cout << "DEBUG-ConnectionSocket-sendData: sended: " << *msg << "; with length: " << sended << std::endl;
}

void ConnectionSocket::sendData(std::fstream *file){
	int sended = 0, s = 0;
	// get length of file:
	file->seekg(0, file->end);
	long msglength = file->tellg();
	file->seekg(0, file->beg);
	std::cout << msglength << std::endl;
	// ...buffer contains the entire file...

	char buffer[BUF];
	while(msglength > sended){
		// read data as a block:
		file->read(buffer,BUF);

		if (file){
			std::cout << "all characters read successfully.\n";
		}else{
			std::cout << "error: only " << file->gcount() << " could be read\n";
			break;
		}

		s = send(this->socketID, &buffer, BUF, 0);
		if(s != -1){
			sended += s;
		}else{
			break;
		}
		std::cout << "msglength: " << msglength << "; sended: " << sended << " send returned: " << s << "; error: " << errno << std::endl;
	}
	std::cout << "DEBUG-ConnectionSocket-sendData: with length: " << sended << std::endl;
}
