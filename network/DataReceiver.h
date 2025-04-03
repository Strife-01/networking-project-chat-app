#include "../utils/BlockingQueue.h"
#include "../utils/Message.h"
#include "../utils/MessageType.h"

class DataReceiver {
private:
	int sock;
	BlockingQueue< Message > *receiverQueue;

public:
	DataReceiver(int sock, BlockingQueue< Message > *receiverQueue);
	void receiveData();
};



