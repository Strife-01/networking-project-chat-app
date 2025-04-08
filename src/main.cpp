#include <thread>
#include <string>
#include <cstring>
#include <vector>

#include "utils/BlockingQueue.h"
#include "backend/network/Client.h"
#include "utils/Message.h"
#include "backend/mac/MediumAccessControl.h"
#include "backend/channel_state/ChannelState.h"

std::string SERVER_ADDR = "netsys.ewi.utwente.nl"; //"127.0.0.1"
int SERVER_PORT = 8954;
int FREQUENCY = 7800;
std::string TOKEN = "cpp-03-7E6C90A2264E5B3996";

using namespace std;

void readInput(BlockingQueue< Message >*senderQueue) {
	while (true) {
		string input;
		getline(cin, input);
		vector<char> char_vec(input.begin(), input.end());
		Message sendMessage;
		if (char_vec.size() > 2) {
			sendMessage = Message(DATA, char_vec);
		}
		else {
			sendMessage = Message(DATA_SHORT, char_vec);
		}		
		senderQueue->push(sendMessage);
	}
}

int main() {
	BlockingQueue< Message > receiverQueue; // Queue messages will arrive in
	BlockingQueue< Message > senderQueue; // Queue for data to transmit
	Client client = Client(SERVER_ADDR, SERVER_PORT, FREQUENCY, TOKEN, &senderQueue, &receiverQueue);

	client.startThread();

	thread inputHandler(readInput, &senderQueue);
	
	// Handle messages from the server / audio framework
	while(true){
		Message temp = receiverQueue.pop(); // wait for a message to arrive
		cout << "Received: " << temp.type << endl;
		switch (temp.type) {
		case DATA: // We received a data frame!
			cout << "DATA: ";
			for (char c : temp.data) {
				cout << c << ",";
			}
			cout << endl;
			break;
		case DATA_SHORT: // We received a short data frame!
			cout << "DATA_SHORT: ";
			for (char c : temp.data) {
				cout << c << ",";
			}
			cout << endl;
			break;
		case FREE: // The channel is no longer busy (no nodes are sending within our detection range)
			Channel_State::chan_state.reset_is_line_busy();
			break;
		case BUSY: // The channel is busy (A node is sending within our detection range)
			Channel_State::chan_state.set_is_line_busy();
			break;
		case SENDING: // This node is sending
            Channel_State::chan_state.set_is_line_busy();
            Channel_State::chan_state.set_is_current_node_sending();
			break;
		case DONE_SENDING: // This node is done sending
			Channel_State::chan_state.reset_is_line_busy();
			Channel_State::chan_state.reset_is_current_node_sending();
            break;
		case END: // Server / audio framework disconnect message. You don't have to handle this
			cout << "END" << endl;
			break;
		case HELLO: // Server / audio framework hello message. You don't have to handle this
			cout << "HELLO" << endl;
			break;
		case TOKEN_ACCEPTED: // Server / audio framework hello message. You don't have to handle this
			cout << "Token Valid!" << endl;
			break;
		case TOKEN_REJECTED: // Server / audio framework hello message. You don't have to handle this
			cout << "Token Rejected!" << endl;
			break;
		}
	}
	
}

