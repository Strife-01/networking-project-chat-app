#include "TransportManager.h"
#include <iostream>
#include "../MessageQueue/MessageQueue.h"

TransportManager::TransportManager(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing), sender(routing), receiver(routing) {

    sender.setSendFunction([this](const std::vector<char>& packet) {
        if (sendFunc) sendFunc(packet);
    });

    receiver.setSendFunction([this](const std::vector<char>& packet) {
        if (sendFunc) sendFunc(packet);
    });

    receiver.setOnMessageReady([this](std::vector<char> message) {
        uint8_t sender_addr = routing->THE_ADDRESSOR_20000.get_my_addr();

        // convert payload into string and queue it
        std::string str_msg(message.begin(), message.end());

        auto final_msg = Message_Queue::msg_queue.create_message(
            sender_addr,
            str_msg,
            false, // not private
            false  // not seen
        );

        Message_Queue::msg_queue.push_message(final_msg);

        // also notify upper layers
        if (onMessageReady) onMessageReady(message);
    });
}

void TransportManager::setSendFunction(std::function<void(const std::vector<char>&)> func) {
    sendFunc = func;
}

void TransportManager::setOnMessageReady(std::function<void(std::vector<char>)> callback) {
    onMessageReady = callback;
}

void TransportManager::sendMessage(std::vector<char> message, uint8_t dest, uint8_t type) {
    uint8_t my_addr = routing->THE_ADDRESSOR_20000.get_my_addr();
    if (!routing->get_routing_table().count(dest)) {
        std::cout << "[TransportManager] No route to destination " << (int)dest << "\n";
        return;
    }

    uint8_t next_hop = routing->get_routing_table()[dest]->next_hop;
    uint8_t msg_id = next_msg_id++;

    auto fragments = Fragmenter::fragmentMessage(
        message, my_addr, dest, next_hop, msg_id, type
    );

    sender.sendFragments(fragments);
}

void TransportManager::onPacketReceived(const std::vector<char>& packet) {
    receiver.onPacketReceived(packet);
}

void TransportManager::onAckReceived(uint8_t msg_id, uint16_t fragment_id) {
    sender.handleAck(msg_id, fragment_id);
}
