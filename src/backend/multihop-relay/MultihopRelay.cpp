#include "MultihopRelay.h"
#include "../addressing/Addressing.h"
#include <vector>

namespace multihop_relay {

    
    void MultihopRelay(){
        
    }

    bool MultihopRelay::is_packet_to_forward(std::vector<char> packet){
        return packet_header::get_separated_header(
            packet_header::bytes_vector_to_header_int(packet)
        ).dest_address != dynamic_addressing::get_my_addr();
    }

    std::vector<char> MultihopRelay::prepare_header_to_forward(std::vector<char> packet, std::map<unsigned char,vector_routing_protocol::Route *> table){
        packet_header::Header h = packet_header::get_separated_header(
            packet_header::bytes_vector_to_header_int(packet)
        );

        h.next_hop_address = table[h.dest_address]->next_hop;
        
        std::vector<char> payload(packet.begin()+4,packet.end());
        return packet_header::add_header_to_payload(h,payload);

    }


}