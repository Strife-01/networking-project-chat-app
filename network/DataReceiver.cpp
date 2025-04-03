#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <sys/socket.h>    	//socket
#include <arpa/inet.h> 		//inet_addr
#endif

#include "DataReceiver.h"
//#include "../utils/BlockingQueue.h"
//#include "../utils/Message.h"

using namespace std;

DataReceiver::DataReceiver(int sock, BlockingQueue< Message > *receiverQueue) {
	this->sock = sock;
	this->receiverQueue = receiverQueue;
}

void DataReceiver::receiveData() {
	char buffer[1500];
	vector<char> message_buff;
	int message_len = -1;
	bool message = false;
	bool shortData = false;
	int len = 0;
	while (1) {		
		len = recv(sock, buffer, sizeof(buffer), 0);
		if (len < 0) {
			cout << "Error receiving data!" << endl;
			return;
		}
		int i = 0;
		//cout << "DATA: ";
		for (i = 0; i < len; i++) {
			uint8_t b = buffer[i];
			//cout << b << " ";

			if (!message) {
				if (b == 0x01) {
					receiverQueue->push(Message(FREE));
				}
				else if (b == 0x04) {
					receiverQueue->push(Message(SENDING));
				}
				else if (b == 0x05) {
					receiverQueue->push(Message(DONE_SENDING));
				}
				else if (b == 0x02) {
					receiverQueue->push(Message(BUSY));
				}
				else if (b == 0x08) {
					receiverQueue->push(Message(END));
				}
				else if (b == 0x09) {
					receiverQueue->push(Message(HELLO));
				}
				else if (b == 0x0A) {
					receiverQueue->push(Message(TOKEN_ACCEPTED));
				}
				else if (b == 0x0B) {
					receiverQueue->push(Message(TOKEN_REJECTED));
				}
				else if (b == 0x03) {
					message = true;
					message_buff.clear();
					message_len = -1;
					shortData = false;
				}
				else if (b == 0x06) {
					message = true;
					message_buff.clear();
					message_len = -1;
					shortData = true;
				}
			} else { // read data into message
				if (message_len == -1) {
					message_len = b;
				} else {
					message_buff.push_back(b);
				}
				if (message_len == message_buff.size()) {
					if (shortData) {
						receiverQueue->push(Message(DATA_SHORT, message_buff));
					}
					else {
						receiverQueue->push(Message(DATA, message_buff));
					}					
					message = false;
				}
			}
			
			
		}
		//cout << endl;
	}

}
