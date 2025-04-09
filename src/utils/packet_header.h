#ifndef HEADER
#define HEADER

#include <cstdint>
#include <vector>
#include "../backend/routing/Route.h"
#include <map>


#define SRC_ADDRESS 0xE0000000
#define NEXT_HOP_ADDRESS 0x1C000000
#define DEST_ADDRESS 0x03800000
#define MORE_FRAGMENTS 0x00400000
#define MESSAGE_ID 0x00380000
#define FRAGMENT_ID 0x0007FF00
#define PAYLOAD_TYPE 0x000000E0
#define PAYLOAD_LENGTH 0x0000001F

namespace packet_header {

    enum types:uint8_t{
        echo,
        data
    };
    
    struct __attribute__((__packed__)) Header {
        uint16_t fragment_id;
        uint8_t source_address;
        uint8_t next_hop_address;
        uint8_t dest_address;
        uint8_t more_fragments;
        uint8_t message_id;
        uint8_t type;
        uint8_t payload_length;
    };

    Header get_separated_header(const uint32_t header);
    std::vector<char> build_header(const Header& h);
    uint32_t bytes_vector_to_header_int(std::vector<char> packet);

    std::vector<char> add_header_to_payload(packet_header::Header h,std::vector<char> payload);
    std::vector<char> prepare_header_to_forward(std::vector<char> packet, std::map<unsigned char,vector_routing_protocol::Route *> table);
}

#endif