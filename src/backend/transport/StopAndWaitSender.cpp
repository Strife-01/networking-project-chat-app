#include "StopAndWaitSender.h"
#include <iostream>
#include <thread>
#include <chrono>

StopAndWaitSender::StopAndWaitSender(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing), ackReceived(false) {}

void StopAndWaitSender::setSendFunction(std::function<void(const std::vector<char>&)> func) {
    sendFunc = func;
}

void StopAndWaitSender::handleAck(uint8_t msg_id, uint16_t fragment_id) {
    std::lock_guard<std::mutex> lock(ackMutex);
    if (msg_id == current_msg_id && fragment_id == current_fragment_id) {
        ackReceived = true;
        ackCond.notify_all();
        std::cout << "[ACK RECEIVED] msg " << (int)msg_id << ", frag " << fragment_id << "\n";
    }
}

void StopAndWaitSender::sendFragments(const std::vector<std::pair<packet_header::Header, std::vector<char>>>& fragments) {
    for (const auto& [header, payload] : fragments) {
        current_msg_id = header.message_id;
        current_fragment_id = header.fragment_id;
        sendWithRetry(header, payload);
    }
}

void StopAndWaitSender::sendWithRetry(packet_header::Header header, const std::vector<char>& payload) {
    const int MAX_RETRIES = 5;
    int attempts = 0;

    uint8_t my_addr = routing->THE_ADDRESSOR_20000.get_my_addr();
    header.source_address = my_addr;

    auto table = routing->get_routing_table();
    if (table.count(header.dest_address)) {
        header.next_hop_address = table[header.dest_address]->next_hop;
    } else {
        std::cout << "[ROUTING ERROR] No route to dest " << (int)header.dest_address << "\n";
        return;
    }

    header.payload_length = payload.size();
    std::vector<char> packet = packet_header::add_header_to_payload(header, payload);

    if (header.source_address != my_addr && header.next_hop_address != my_addr) {
        std::cout << "[SKIP] Not source or next hop\n";
        return;
    }

    {
        std::lock_guard<std::mutex> lock(ackMutex);
        ackReceived = false;
    }

    while (attempts < MAX_RETRIES) {
        std::cout << "[SEND] msg " << (int)header.message_id
                  << ", frag " << header.fragment_id
                  << ", attempt " << (attempts + 1) << "\n";

        if (sendFunc) {
            sendFunc(packet);
        }

        std::unique_lock<std::mutex> lock(ackMutex);
        if (ackCond.wait_for(lock, std::chrono::milliseconds(500), [this] { return ackReceived.load(); })) {
            std::cout << "[DELIVERED] Fragment " << header.fragment_id << "\n";
            return;
        }

        attempts++;
    }

    std::cout << "[FAIL] Gave up on msg " << (int)header.message_id << ", frag " << header.fragment_id << "\n";
}
