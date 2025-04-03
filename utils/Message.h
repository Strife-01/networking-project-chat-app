#ifndef Included_Message
#define Included_Message

#include "MessageType.h"

class Message {
	
public:
	MessageType type;
	vector<char> data;

	Message() {
	}

	Message(MessageType newType) {
		type = newType;
	}

	Message(MessageType newType, vector<char> newData) {
		type = newType;
		data = newData;
	}
};

#endif