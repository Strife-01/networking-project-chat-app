#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <netdb.h>
#include <sys/types.h>
#endif

#include <list>
#include "../../utils/BlockingQueue.h"
#include "../../utils/Message.h"
#include "Client.h"
#include "DataReceiver.h"

using namespace std;

Client::Client(string server_addr, int server_port, int frequency, string token, BlockingQueue< Message > *senderQueue, BlockingQueue< Message > *receiverQueue) {
	this->server_addr = server_addr;
	this->server_port = server_port;
	this->frequency = frequency;
	this->token = token;
	this->senderQueue = senderQueue;
	this->receiverQueue = receiverQueue;
}

int Client::openSocket() {
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// make socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "error opening socket!" << endl;
		WSACleanup();
		return -1;
	}
#elif __linux__
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		cout << "Error opening socket!" << endl;
		return -1;
	}
#endif

	struct addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	struct addrinfo *results;
	string port = to_string(server_port);

	if (getaddrinfo(server_addr.c_str(), port.c_str(), &hints, &results) < 0) {
		cout << "Could not resolve hostname!" << endl;
#ifdef _WIN32
		WSACleanup();
#endif
		return -1;
	}

#ifdef _WIN32
	if (connect(sock, results->ai_addr, results->ai_addrlen) == SOCKET_ERROR) {
		cout << "Could not connect to server!" << endl;
		WSACleanup();
		return -1;
	}
#elif __linux__
	if (connect(sock, results->ai_addr, results->ai_addrlen) < 0) {
		cout << "Could not connect to server!" << endl;
		return -1;
	}
#endif
	return sock;
}

void Client::listener(int sock){
	// Send hello to server.
	char hello_buff[4];
	hello_buff[0] = 9;
	hello_buff[1] = (frequency >> 16) & 0xff;
	hello_buff[2] = (frequency >> 8) & 0xff;
	hello_buff[3] = frequency & 0xff;
	send(sock, hello_buff, 4, 0);

	// Send token to server
	char token_buff[this->token.length()+2];
	token_buff[0] = 10;
	token_buff[1] = this->token.length();
	std::copy(this->token.begin(), this->token.end(), &token_buff[2]);
	send(sock, token_buff, this->token.length()+2, 0);

	// Socket connection made, start receiving threads!
	DataReceiver dataReceiver = DataReceiver(sock, receiverQueue);
	thread receiver(&DataReceiver::receiveData, dataReceiver);
	receiver.detach();

	// enter sender loop
	Message toSend;
	char buff[1024];
	while (true) {
		toSend = senderQueue->pop();
		if (toSend.type == DATA_SHORT) {
			buff[0] = 6;
		}
		else {
			buff[0] = 3;
		}		
		buff[1] = (char)toSend.data.size();
		//memcpy( (void*)buff[2], toSend.data(), toSend.size() );
		if (toSend.data.size() < 1000) {
			copy(toSend.data.begin(), toSend.data.end(), &buff[2]);
			send(sock, buff, toSend.data.size() + 2, 0);
		}
	}
}

void Client::startThread() {
	int sock = openSocket();
	if (sock < 0) return;
	thread client_thread(&Client::listener, this, sock);
	client_thread.detach();
}