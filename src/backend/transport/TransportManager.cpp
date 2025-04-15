#include "TransportManager.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>
#include "../MessageQueue/MessageQueue.h"
#include "StopAndWaitSender.h"

TransportManager::TransportManager(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing), sender(routing), receiver(routing) {

    sender.setSendFunction([this](const std::vector<char>& packet) {
        if (sendFunc) sendFunc(packet);
    });

    receiver.setSendFunction([this](const std::vector<char>& packet) {
        if (sendFunc) sendFunc(packet);
    });

    receiver.setOnMessageReady([this](uint8_t addr,std::vector<char> message,bool broadcast) {
        uint8_t sender_addr = this->routing->THE_ADDRESSOR_20000.get_my_addr();

        std::string str_msg(message.begin(), message.end());

        auto final_msg = Message_Queue::msg_queue.create_message(
            sender_addr,
            str_msg,
            true,  // not private
            false  // not seen
        );

        Message_Queue::msg_queue.push_message(final_msg, sender_addr);

        if (onMessageReady) onMessageReady(sender_addr, message,broadcast);
    });
}

void TransportManager::setSendFunction(std::function<void(const std::vector<char>&)> func) {
    sendFunc = func;
}

void TransportManager::setOnMessageReady(std::function<void(uint8_t addr,std::vector<char>,bool broadcast)> callback) {
    onMessageReady = callback;
}

void TransportManager::sendMessage(std::vector<char> message, uint8_t dest, uint8_t type) {
    uint8_t msg_id = next_msg_id++;


    // for broadcast, no routing lookup
    if (dest != 0) {
        if (!routing->get_routing_table().count(dest)) {
            std::cout << "[TransportManager] No route to destination " << (int)dest << "\n";
            return;
        }
    }

    thread wts(wait_to_send,routing,message,dest,type,msg_id, &sender);
    wts.detach();
}

void TransportManager::wait_to_send(vector_routing_protocol::VectorRoutingProtocol * routing,std::vector<char> message, uint8_t dest, uint8_t type, uint8_t msg_id, StopAndWaitSender * sender){
    uint8_t next_hop = 0;
    uint8_t my_addr = routing->THE_ADDRESSOR_20000.get_my_addr();

    if(dest != 0){
        do{

            next_hop = routing->myRoutingTable[dest]->next_hop;
    
            // protection in case of table TTL trigger row reset right before sending
            if(routing->myRoutingTable[dest]->cost >= INFINITY_COST){
                this_thread::sleep_for(chrono::seconds(1));
            }
    
        }while(routing->myRoutingTable[dest]->cost >= INFINITY_COST);
    }


    printf("\t[i] Next hop to %d is %d",dest,next_hop);

    auto fragments = Fragmenter::fragmentMessage(
        message, my_addr, dest, next_hop, msg_id, type
    );

    sender->sendFragments(fragments);
}

void TransportManager::onPacketReceived(const std::vector<char>& packet) {
    receiver.onPacketReceived(packet);
}

void TransportManager::onAckReceived(std::vector<char> packet) {
    sender.handleAck(packet);
}
