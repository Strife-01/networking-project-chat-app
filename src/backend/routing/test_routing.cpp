#include <bitset>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include "VectorRoutingProtocol.h"


/*
TODO : see why returned table is empty; check if table serialization still stands

*/

// Mock classes or functions if necessary

void test_calculate_link_cost_from_rtt() {
    vector_routing_protocol::VectorRoutingProtocol protocol;
    int RTT = 10;
    uint32_t cost = protocol.calculate_link_cost_from_rtt(RTT);
    assert(cost == 1);  
}

void test_process_payload() {

    puts("[+] TEST 1 ...");
    vector_routing_protocol::VectorRoutingProtocol protocol;

    
    packet_header::Header pkt;
    pkt.dest_address = 2;
    pkt.source_address = 1;
    pkt.next_hop_address = 2;
    pkt.fragment_id = 1;
    pkt.more_fragments = 0;
    pkt.message_id = 1;
    pkt.payload_length = 8; // (4*2), 4 nodes, 2 1 byte fields per node => 8 bytes
    pkt.type = 1; // not defined yet, put 1 as test type


    // [HEADER] + [DEST][COST]...
    std::vector<char> payload;
    std::vector<char> h_arr = packet_header::build_header(pkt);
    // 4 bytes header
    for(int i = 0;i<4;i++){
        payload.push_back(h_arr[i]);
    }
    printf("built header\n");

    //protocol.print_pkt_header(protocol.extract_header(payload));

    
    // simulation of table from node 1 sent to node 2
    // node 1 knows node 2 and at cost 1
    // node 1 knows itself at cost 0
    // node 1 do not know the other nodes (infinity)
    payload.push_back(0x01);
    payload.push_back(0x00);
    payload.push_back(0x02);
    payload.push_back(0x01);
    payload.push_back(0x03);
    payload.push_back(INFINITY_COST);
    payload.push_back(0x04);
    payload.push_back(INFINITY_COST);


    std::map<unsigned char, vector_routing_protocol::Route*> table = protocol.process_payload(payload);

    protocol.print_table(table);
    
    assert(table.size() == 4);
    assert(table[1]->destination_node == 1);
    assert(table[1]->cost == 0);

    puts("[v] TEST 1 PASSED");
}

void test_serialize_table() {

    puts("[+] TEST 2...");
    vector_routing_protocol::VectorRoutingProtocol protocol;
    std::map<unsigned char, vector_routing_protocol::Route*> table;
    vector_routing_protocol::Route* route = (vector_routing_protocol::Route*)malloc(sizeof(vector_routing_protocol::Route));
    route->destination_node = 1;
    route->cost = 2;
    table[1] = route;

    std::vector<char> payload = protocol.serialize_table(table);
    printf("payload size = %zu\n",payload.size());
    assert(payload.size() == 2);
    assert(payload[0] == 1);
    assert(payload[1] == 2);
    
    puts("[v] TEST 2 PASSED");

    free(route);
}

void test_register_echo() {

    puts("[+] TEST 3...");

    vector_routing_protocol::VectorRoutingProtocol protocol;
    puts("Internal routing table after initialization :");
    protocol.print_interntal_table();

    packet_header::Header pkt;
    pkt.dest_address = 2;
    pkt.source_address = 1;
    pkt.next_hop_address = 2;
    pkt.fragment_id = 1;
    pkt.more_fragments = 0;
    pkt.message_id = 1;
    pkt.payload_length = 8; // (4*2), 4 nodes, 2 1 byte fields per node => 8 bytes
    pkt.type = 1; // not defined yet, put 1 as test type

    // we still are node 2 receiving from node one as in test 2 
    std::vector<char> payload = {
        0x01, 
        0x00, 
        0x02,
        0x01, 
        0x03, 
        INFINITY_COST, 
        0x04, 
        INFINITY_COST, 
    };

    auto serialized_header = packet_header::build_header(pkt);
    payload.insert(
        payload.begin(),
        serialized_header.begin(),
        serialized_header.end()
    );



    protocol.register_echo(payload);

    // Vérifiez que la table de routage a été mise à jour correctement
    auto routingTable = protocol.get_routing_table();
    assert(routingTable.size() == 4);
    assert(routingTable[1]->cost == 1);

    // Nettoyage de la mémoire allouée
    for (auto& entry : routingTable) {
        free(entry.second);
    }

    puts("[v] TEST 3 PASSED");

}

int main() {
    test_calculate_link_cost_from_rtt();
    test_process_payload();
    test_serialize_table();
    test_register_echo();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
