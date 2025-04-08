//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "packet_header.h"

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