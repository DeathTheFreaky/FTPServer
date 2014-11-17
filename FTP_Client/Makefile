CC=g++
CFLAGS=-ggdb -Wall -Werror --std=c++11

all: FTP_Client

FTP_Client: clean copy main sock client
	mv *.o compile
	$(CC) compile/FTP_Client.o compile/Socket.o compile/Client.o -o FTP_Client

copy:	src
	mkdir compile
	cp -R src/ compile
	cd compile

main: src/FTP_Client.cpp
	$(CC) $(CFLAGS) -c src/FTP_Client.cpp

sock: src/Socket.cpp
	$(CC) $(CFLAGS) -c src/Socket.cpp

client: src/Client.cpp
	$(CC) $(CFLAGS) -c src/Client.cpp

clean:
	rm -f -R compile
	rm -f FTP_Client

