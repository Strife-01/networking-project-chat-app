//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "packet_header.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

packet_header::Header packet_header::get_separated_header(const uint32_t header) {
    Header result;
    result.source_address = static_cast<uint8_t>((header & SRC_ADDRESS) >> 29);
    result.next_hop_address = static_cast<uint8_t>((header & NEXT_HOP_ADDRESS) >> 26);
    result.dest_address = static_cast<uint8_t>((header & DEST_ADDRESS) >> 23);
    result.more_fragments = static_cast<uint8_t>((header & MORE_FRAGMENTS) >> 22);
    result.message_id = static_cast<uint8_t>((header & MESSAGE_ID) >> 19);
    result.fragment_id = static_cast<uint16_t>((header & FRAGMENT_ID) >> 8);
    result.type = static_cast<uint8_t>((header & PAYLOAD_TYPE) >> 5);
    result.payload_length = static_cast<uint8_t>(header & PAYLOAD_LENGTH);
    return result;
}


std::vector<char> packet_header::build_header(const Header& h) {
    uint32_t header = 0;
    header |= (static_cast<uint32_t>(h.source_address) << 29) & SRC_ADDRESS;
    header |= (static_cast<uint32_t>(h.next_hop_address) << 26) & NEXT_HOP_ADDRESS;
    header |= (static_cast<uint32_t>(h.dest_address) << 23) & DEST_ADDRESS;
    header |= (static_cast<uint32_t>(h.more_fragments) << 22) & MORE_FRAGMENTS;
    header |= (static_cast<uint32_t>(h.message_id) << 19) & MESSAGE_ID;
    header |= (static_cast<uint32_t>(h.fragment_id) << 8) & FRAGMENT_ID;
    header |= (static_cast<uint32_t>(h.type) << 5) & PAYLOAD_TYPE;
    header |= static_cast<uint32_t>(h.payload_length) & PAYLOAD_LENGTH;

    std::vector<char> ret;
    ret.push_back(static_cast<char>((header & 0xFF000000)>>24));
    ret.push_back(static_cast<char>((header & 0x00FF0000)>>16));
    ret.push_back(static_cast<char>((header & 0x0000FF00)>>8));
    ret.push_back(static_cast<char>(header & 0x000000FF));

    return ret;
}


uint32_t packet_header::bytes_vector_to_header_int(std::vector<char> packet){
    return (packet[0] << 24) | (packet[1] << 16) | (packet[2] << 8) | packet[3];
}

std::vector<char> packet_header::add_header_to_payload(packet_header::Header h,std::vector<char> payload){
    
    std::vector<char> sr_header = packet_header::build_header(h);
    payload.insert(payload.begin(),sr_header.begin(),sr_header.end());
    return payload;
}



//typedef union {
//    unsigned int header;
//    struct {
//        unsigned char payload_len : 5;
//        unsigned char type : 3;
//        unsigned short fragment_id : 11;
//        unsigned char msg_id : 3;
//        bool MF : 1;
//        unsigned char dst_addr : 3;
//        unsigned char next_hop : 3;
//        unsigned char src_addr : 3;
//    } fields;
//} packet_header;