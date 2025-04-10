// WE DON'T HANDLE BROADCAST'

#include "StopAndWaitReceiver.h"
#include <iostream>

StopAndWaitReceiver::StopAndWaitReceiver(vector_routing_protocol::VectorRoutingProtocol* routing)
    : routing(routing) {}

void StopAndWaitReceiver::setSendFunction(std::function<void(const std::vector<char>&)> callback) {
    sendFunc = callback;
}

void StopAndWaitReceiver::setOnMessageReady(std::function<void(std::vector<char>)> callback) {
    onMessageReady = callback;
}

void StopAndWaitReceiver::sendAck(uint8_t msg_id, uint16_t frag_id, uint8_t dest) {
    packet_header::Header ack;
    // here we create an ack header
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
    if (sendFunc) sendFunc(ack_packet);
}

void StopAndWaitReceiver::onPacketReceived(const std::vector<char>& packet) {
    if (packet.size() < 4) return;

    uint32_t raw_header = packet_header::bytes_vector_to_header_int(packet);
    packet_header::Header header = packet_header::get_separated_header(raw_header);

    uint8_t me = routing->THE_ADDRESSOR_20000.get_my_addr();

    // FORWARDING CASE - we don't send an ack if we're forwarding
    if (header.next_hop_address == me && header.dest_address != me) {
        std::vector<char> forwarded = relay.prepare_header_to_forward(packet, routing->get_routing_table());
        if (sendFunc) sendFunc(forwarded);
        std::cout << "[FORWARD] Fragment " << header.fragment_id << " forwarded.\n";
        return;
    }

    // IGNORE IF NOT FOR US
    if (header.dest_address != me && header.next_hop_address != me) {
        return;
    }

    // AT DESTINATION
    std::vector<char> payload(packet.begin() + 4, packet.end());

    sendAck(header.message_id, header.fragment_id, header.source_address);

    std::vector<char> full = reassembler.insertFragment(header.message_id, header.fragment_id,
                                                        header.more_fragments, payload);

    if (!full.empty() && onMessageReady) {
        std::cout << "[DELIVERED] Full message received (msg " << (int)header.message_id << ")\n";
        onMessageReady(full);
    }
}
