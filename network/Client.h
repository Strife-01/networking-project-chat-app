#include "../utils/Message.h"

class Client {

private:
	string server_addr;
	int server_port;
	int frequency;
	string token;
	BlockingQueue< Message > *senderQueue;
	BlockingQueue< Message > *receiverQueue;

	int openSocket();

public:
	Client(string server_addr, int server_port, int frequency, string token, BlockingQueue< Message > *senderQueue, BlockingQueue< Message > *receiverQueue);

	void startThread();
	void listener(int sock);
};
