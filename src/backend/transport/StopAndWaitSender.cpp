#include "StopAndWaitSender.h"
#include "StopAndWaitReceiver.h"
#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

/* Fragments a message into packets
 * Sends each packet to the next next_hop
 * waits for an ack from the destination
 * retransmits if timeout
 */

StopAndWaitSender::StopAndWaitSender(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing), ackReceived(false) {}

void StopAndWaitSender::setSendFunction(std::function<void(const std::vector<char>&)> func) {
   // here implement the actual sender function
    sendFunc = func;
}

void StopAndWaitSender::handleAck(std::vector<char> packet) {
    std::lock_guard<std::mutex> lock(ackMutex);
    multihop_relay::MultihopRelay relay;

    packet_header::Header h = packet_header::get_separated_header(
        packet_header::bytes_vector_to_header_int(packet)
    );

    uint8_t my_addr = dynamic_addressing::get_my_addr();
    if ((h.dest_address != my_addr) && (h.next_hop_address == my_addr)){
        std::vector<char> forwarded = relay.prepare_header_to_forward(packet, routing->get_routing_table());
        if (sendFunc) sendFunc(forwarded);
        std::cout << "[FORWARD] ACK for" << h.dest_address << "\n";
    }

    if (h.message_id == current_msg_id && h.fragment_id == current_fragment_id) {
        ackReceived = true;
        ackCond.notify_all();
        std::cout << "[ACK RECEIVED] msg " << (int) h.message_id << ", frag " << h.fragment_id << "\n";
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
            thread sf(sendFunc,packet);
            sf.detach();
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
