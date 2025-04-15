#include "StopAndWaitSender.h"
#include "StopAndWaitReceiver.h"
#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "../../utils/Random.h"

/**
 * Fragments a message into packets
 * Sends each packet to the next_hop
 * waits for an ack for the destination (IF IT'S NOT A BROADCAST)
 * retransmits if timeout (if not broacast)
 */

StopAndWaitSender::StopAndWaitSender(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing), ackReceived(false) {}

void StopAndWaitSender::setSendFunction(std::function<void(const std::vector<char>&)> func) {
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
        if (sendFunc){
            thread sf(sendFunc,forwarded);
            sf.detach();
        }
        std::cout << "[FORWARD] ACK for " << (int)h.dest_address << "\n";
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
    
    if(!vector_routing_protocol::VectorRoutingProtocol::is_protocol_paused()){
        vector_routing_protocol::VectorRoutingProtocol::pause_protocol();
    }


    const int MAX_RETRIES = 20;
    int attempts = 0;

    uint8_t my_addr = routing->THE_ADDRESSOR_20000.get_my_addr();
    header.source_address = my_addr;

    // handle broadcast case (dest = 0) -> HOW TO STOP A BROADCAST
    bool isBroadcast = (header.dest_address == 0);
    if (isBroadcast) {

        
        std::thread bc(sendBroadcast,this,header,payload);
        // join to still send fragment by fragment
        bc.join();

        return; // if it's broadcast, don't wait for acks
    }

    // normal unicast routing logic
    auto table = routing->get_routing_table();
    if (table.count(header.dest_address)) {
        header.next_hop_address = table[header.dest_address]->next_hop;
    } else {
        std::cout << "[ROUTING ERROR] No route to dest " << (int)header.dest_address << "\n";
        return;
    }

    header.payload_length = payload.size();
    std::vector<char> packet = packet_header::add_header_to_payload(header, payload);

    {
        std::lock_guard<std::mutex> lock(ackMutex);
        ackReceived = false;
    }

    while (attempts < MAX_RETRIES) {
        std::cout << "[SEND] msg " << (int)header.message_id
                  << ", frag " << header.fragment_id
                  << ", attempt " << (attempts + 1) << "\n";

        if (sendFunc) {
            std::thread sf(sendFunc, packet);
            sf.detach();
        }

        std::unique_lock<std::mutex> lock(ackMutex);
        if (ackCond.wait_for(lock, std::chrono::milliseconds(TIMEOUT), [this] { return ackReceived.load(); })) {
            std::cout << "[DELIVERED] Fragment " << header.fragment_id << "\n";
            return;
        }

        attempts++;
    }

    std::cout << "[FAIL] Gave up on msg " << (int)header.message_id << ", frag " << header.fragment_id << "\n";

    vector_routing_protocol::VectorRoutingProtocol::resume_protocol();
}


void StopAndWaitSender::sendBroadcast(StopAndWaitSender* sender_obj,packet_header::Header header,const std::vector<char>&payload){
    int tries_count = 1;
    while(tries_count<MAX_BCAST_TRIES){
        header.next_hop_address = 0;
        header.payload_length = payload.size();
        std::vector<char> packet = packet_header::add_header_to_payload(header, payload);

        std::cout << "[BROADCAST] Sending broadcast fragment " << header.fragment_id << "\n\t" << "try n°"<<tries_count<<std::endl;

        
        if (sender_obj->sendFunc) {
            std::thread sf(sender_obj->sendFunc, packet);
            sf.detach();
        }

        ++tries_count; 

        this_thread::sleep_for(chrono::milliseconds(Random::get(MIN_BCAST_TRIES_SLEEP,MAX_BCAST_TRIES_SLEEP)));
    }
}
