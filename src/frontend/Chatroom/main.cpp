#include "chatroomwindow.h"
#include <QApplication>
#include <cstdio>
#include <thread>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#include "../../utils/BlockingQueue.h"
#include "../../backend/network/Client.h"
#include "../../utils/Message.h"
#include "../../backend/mac/MediumAccessControl.h"
#include "../../backend/channel_state/ChannelState.h"
#include "../../backend/routing/VectorRoutingProtocol.h"
#include "../../utils/packet_header.h"
#include "../../backend/transport/TransportManager.h"
#include "../../backend/MessageQueue/MessageQueue.h"

std::string SERVER_ADDR = "netsys.ewi.utwente.nl"; //"127.0.0.1"
int SERVER_PORT = 8954;
int FREQUENCY = 7800;
std::string TOKEN = "cpp-03-7E6C90A2264E5B3996";
using namespace std;


void real_main(ChatRoomWindow * w){
    BlockingQueue< Message > receiverQueue; // Queue messages will arrive in
    BlockingQueue< Message > senderQueue;   // Queue for data to transmit

    Client client = Client(SERVER_ADDR, SERVER_PORT, FREQUENCY, TOKEN, &senderQueue, &receiverQueue);
    client.startThread();

    /*printf("Insert static addr >>");
    unsigned int node_addr;
    scanf("%d", &node_addr);
    getchar();
    printf("selected address : %d\n", node_addr);
    dynamic_addressing::set_my_addr(node_addr);*/

    vector_routing_protocol::VectorRoutingProtocol v_r_proto(&senderQueue);
    v_r_proto.start_ticking_thread();

    // transport Layer
    TransportManager transportManager(&v_r_proto);

    // passing transport manager to the window
    w->tm =&transportManager;

    // send outgoing transport packets using senderQueue
    transportManager.setSendFunction([&](const std::vector<char>& packet) {
        bool sent = false;
        int ttw = Random::get(0,100);
        puts("trying to send");
        while(!sent){

            if(!Channel_State::chan_state.get_is_line_busy())
            {

                // sleep a random amount of time before sending

                this_thread::sleep_for(chrono::milliseconds(ttw));

                if(!Channel_State::chan_state.get_is_line_busy()

                    ){
                    puts("TIME WINDOW OPEN");
                    senderQueue.push(Message(DATA, packet));
                    sent = true;
                }

            }
        }

        puts("message actually sent");

        pthread_exit(0);
    });

    // handle full reassembled messages and queue them
    transportManager.setOnMessageReady([&w](uint8_t addr,std::vector<char> msg) {
        std::string str(msg.begin(), msg.end());

        if(addr == 0){
            w->receiveGlobalMessage(addr,msg);
        }else{
            w->receivePrivateMessage(addr,msg);
        }

        std::cout << "[DELIVERED] Full message: " << str << std::endl;
    });



    // handle messages from the server / audio framework
    while (true) {
        Message temp = receiverQueue.pop(); // wait for a message to arrive
        //receiverQueue.push(Message(DATA, full_packet));

        packet_header::Header h;

        switch (temp.type) {
        case DATA: // We received a data frame!
            h = packet_header::get_separated_header(
                packet_header::bytes_vector_to_header_int(temp.data)
                );

            // ECHO REQUEST HANDLING
            if (h.type == packet_header::echo) {
                puts("RECEIVED ECHO");
                w->updateMemberList();
                v_r_proto.register_echo(temp.data);
            }
            // handle DATA packet
            else if (h.type == packet_header::data) {
                transportManager.onPacketReceived(temp.data);
            }
            // handle ACK
            else if (h.type == packet_header::ack) {
                transportManager.onAckReceived(temp.data);
            }

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

        auto delivered = Message_Queue::msg_queue.get_messages();
        for (auto& m : delivered) {
            std::cout << "[MSG] From " << (int)m.sender_address << ": " << m.message << "\n";
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatRoomWindow w;

    w.show();
    thread f(real_main,&w);
    f.detach();

    return a.exec();
}

