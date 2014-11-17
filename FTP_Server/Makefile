CC=g++
CFLAGS=-ggdb -Wall -Werror --std=c++11
TFLAGS= -pthread
LDAPFLAGS=-DLDAP_DEPRECATED -lldap

all: FTP_Server

FTP_Server: clean copy main cmdh conns file filemang ldap mains serv threadmang
	mv *.o compile
	$(CC) compile/FTP_Server.o compile/CommandHandler.o compile/ConnectionSocket.o compile/File.o compile/Filemanager.o compile/LDAPConnection.o compile/MainSocket.o compile/Server.o compile/Threadmanager.o -o FTP_Server $(LDAPFLAGS)

copy:	src
	mkdir compile
	cp -R src/ compile
	cd compile

main: src/FTP_Server.cpp
	$(CC) $(CFLAGS) $(TFLAGS) -c src/FTP_Server.cpp

cmdh: src/CommandHandler.cpp
	$(CC) $(CFLAGS) -c src/CommandHandler.cpp

conns: src/ConnectionSocket.cpp
	$(CC) $(CFLAGS) $(TFLAGS) -c src/ConnectionSocket.cpp

file: src/File.cpp
	$(CC) $(CFLAGS) -c src/File.cpp

filemang: src/Filemanager.cpp
	$(CC) $(CFLAGS) -c src/Filemanager.cpp

ldap: src/LDAPConnection.cpp
	$(CC) $(CFLAGS) -c src/LDAPConnection.cpp $(LDAPFLAGS)

mains: src/MainSocket.cpp
	$(CC) $(CFLAGS) -c src/MainSocket.cpp

serv: src/Server.cpp
	$(CC) $(CFLAGS) -c src/Server.cpp

threadmang: src/Threadmanager.cpp
	$(CC) $(CFLAGS) $(TFLAGS) -c src/Threadmanager.cpp

clean:
	rm -f -R compile
	rm -f FTP_Server
