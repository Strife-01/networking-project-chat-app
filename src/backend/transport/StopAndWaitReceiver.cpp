#include "StopAndWaitReceiver.h"
#include "StopAndWaitSender.h"
#include <iostream>
#include <thread>
#include <utility>
#include "../../utils/Random.h"

/*
 *  Receives packets
 * if it's the next hop, forwards the packet
 * if it's the destination, send an ack packet
 * reassebmles fragments
 * if it's a broadcast, don't send an ack
 */


StopAndWaitReceiver::StopAndWaitReceiver(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing) {}

void StopAndWaitReceiver::setSendFunction(std::function<void(const std::vector<char>&)> callback) {
    sendFunc = callback;
}

void StopAndWaitReceiver::setOnMessageReady(std::function<void(uint8_t addr,std::vector<char> message,bool broadcast)> callback) {
    // a message is ready when we have received all the fragments
    onMessageReady = callback;
}

void StopAndWaitReceiver::sendAck(uint8_t msg_id, uint16_t frag_id, uint8_t dest) {
    packet_header::Header ack;
    // create ack header
    ack.message_id = msg_id;
    ack.fragment_id = frag_id;
    ack.type = packet_header::types::ack;
    ack.source_address = routing->THE_ADDRESSOR_20000.get_my_addr();
    ack.dest_address = dest;

    auto table = routing->get_routing_table();
    if (table.count(dest)) {
        ack.next_hop_address = table[dest]->next_hop;
    } else {
        std::cout << "[ACK FAIL] No route to " << (int)dest << "\n";
        return;
    }

    ack.more_fragments = 0;
    ack.payload_length = 0;

    std::vector<char> ack_packet = packet_header::build_header(ack);
    if (sendFunc) {
        thread sf(sendFunc, ack_packet);
        sf.detach();
    }
}

void StopAndWaitReceiver::onPacketReceived(const std::vector<char>& packet) {
    if (packet.size() < 4) return;

    uint32_t raw_header = packet_header::bytes_vector_to_header_int(packet);
    packet_header::Header header = packet_header::get_separated_header(raw_header);
    uint8_t me = routing->THE_ADDRESSOR_20000.get_my_addr();

    //vector_routing_protocol::VectorRoutingProtocol::print_pkt_header(header);

    // BROADCAST HANDLING
    bool isBroadcast = (header.dest_address == 0);
    std::vector<char> payload(packet.begin() + 4, packet.begin()+header.payload_length+4);

    if (isBroadcast) {

        std::cout << "[BROADCAST] Received fragment " << header.fragment_id << " from " << (int)header.source_address << "\n";

        auto id = std::make_pair(header.message_id,static_cast<unsigned short int>(header.fragment_id));
        bool already_received = seenBroadcasts.count(id) > 0; // already seen message
        
        if(!already_received){
            seenBroadcasts.insert(id);
            std::thread bc(sendBroadcast,this,header,payload);
            bc.join();
        }else{
            return;
        }
        
    }

    // FORWARDING CASE - we don't send an ack if it's a broadcast or if we are forwarding
    if ((header.next_hop_address == me) && !isBroadcast) {
        if (header.dest_address != me) {
            std::vector<char> forwarded = relay.prepare_header_to_forward(packet, routing->get_routing_table());
            if (sendFunc) {
                std::thread sf(sendFunc, forwarded);
                sf.detach();
            }
            std::cout << "[FORWARD] Fragment " << header.fragment_id << " forwarded.\n";
            return;
        }
    }

    // IGNORE if not for us (and not broadcast)
    if (!isBroadcast && header.next_hop_address != me && header.dest_address != me) {
        return;
    }


    // only send ACKs if this is not a broadcast
    if (!isBroadcast) {
        sendAck(header.message_id, header.fragment_id, header.source_address);
    }


    
    std::vector<char> full = reassembler.insertFragment(header.message_id, header.fragment_id,
                                                        header.more_fragments, payload);
    
    
    if (!full.empty() && onMessageReady) {
        std::cout << "[DELIVERED] " << (isBroadcast ? "Broadcast" : "Unicast")
                  << " message received (msg " << (int)header.message_id << ")\n";
        onMessageReady(header.source_address,full,isBroadcast);
    }


}


void StopAndWaitReceiver::sendBroadcast(StopAndWaitReceiver* sender_obj,packet_header::Header header,const std::vector<char>&payload){
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
