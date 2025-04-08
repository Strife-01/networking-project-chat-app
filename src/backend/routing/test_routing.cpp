#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include "VectorRoutingProtocol.h"

// Mock classes or functions if necessary

void test_calculate_link_cost_from_rtt() {
    vector_routing_protocol::VectorRoutingProtocol protocol;
    int RTT = 10;
    uint32_t cost = protocol.calculate_link_cost_from_rtt(RTT);
    assert(cost == 1);  // Assurez-vous que cela correspond à la logique de votre fonction
}

void test_process_payload() {
    vector_routing_protocol::VectorRoutingProtocol protocol;

    
    packet_header pkt;
    pkt.fields.dst_addr = 2;
    pkt.fields.src_addr = 1;
    pkt.fields.next_hop = 2;
    pkt.fields.fragment_id = 1;
    pkt.fields.MF = 0;
    pkt.fields.msg_id = 1;
    pkt.fields.payload_len = 8; // (4*2), 4 nodes, 2 1 byte fields per node => 8 bytes
    pkt.fields.type = 1; // not defined yet, put 1 as test type

    protocol.print_pkt_header(pkt);



    // [HEADER] + [DEST][COST]...
    std::vector<char> payload;
        
    // 4 bytes header
    for(int i = 3;i>=0;i--){
        std::bitset<8>  x(pkt.bytes[i]);
        std::cout<<"byte="<<x;
        printf(" ; %d\n",i);
        payload.push_back(pkt.bytes[i]); // WTF IT REVERSES THE BYTE ORDER WHEN ASSIGNED
        
    }

    
    payload.push_back(0x01);
    payload.push_back(0x00);
    payload.push_back(0x02);
    payload.push_back(0x01);
    payload.push_back(0x03);
    payload.push_back(INFINITY_COST);
    payload.push_back(0x04);
    payload.push_back(INFINITY_COST);


    std::map<uint32_t, vector_routing_protocol::Route*> table = protocol.process_payload(payload);

    protocol.print_table();
    
    assert(table.size() == 1);
    assert(table[0]->destination_node == 1);
    assert(table[0]->cost == 1);

    // Nettoyage de la mémoire allouée
    for (auto& entry : table) {
        free(entry.second);
    }
}

void test_serialize_table() {
    vector_routing_protocol::VectorRoutingProtocol protocol;
    std::map<uint32_t, vector_routing_protocol::Route*> table;
    vector_routing_protocol::Route* route = (vector_routing_protocol::Route*)malloc(sizeof(vector_routing_protocol::Route));
    route->destination_node = 1;
    route->cost = 2;
    table[1] = route;

    std::vector<char> payload = protocol.serialize_table(table);

    assert(payload.size() == 2 * sizeof(uint32_t));
    assert(*(uint32_t*)&payload[0] == 1);
    assert(*(uint32_t*)&payload[sizeof(uint32_t)] == 2);

    free(route);
}

void test_register_echo() {
    vector_routing_protocol::VectorRoutingProtocol protocol;
    std::vector<char> payload = {0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
    protocol.register_echo(payload);

    // Vérifiez que la table de routage a été mise à jour correctement
    auto routingTable = protocol.get_routing_table();
    assert(routingTable.size() == 1);
    assert(routingTable[1]->cost == 1);

    // Nettoyage de la mémoire allouée
    for (auto& entry : routingTable) {
        free(entry.second);
    }
}

int main() {
    test_calculate_link_cost_from_rtt();
    test_process_payload();
    test_serialize_table();
    test_register_echo();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
