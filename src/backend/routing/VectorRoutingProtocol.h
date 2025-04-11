#ifndef V_ROUTING 
#define V_ROUTING

#include <cstdint>
#include <map>
#include <vector>
#include "Route.h"
#include "../../utils/packet_header.h"
#include "../addressing/Addressing.h"
#include "../../utils/BlockingQueue.h"
#include "../../utils/Message.h"

#define BROADCAST_ADDR 0
#define MAXIMUM_COST 4
#define INFINITY_COST 5
#define MAX_NODE_NUMBER 4
#define MAX_TTL 100
#define BROADCAST_TIMEOUT 5
#define MEAN_RTT 100 // implement a real computation later
#define TICK_TIME 1000

namespace vector_routing_protocol {

    class VectorRoutingProtocol {
    public:
        VectorRoutingProtocol(BlockingQueue< Message >* senderQueue);
        void register_echo(std::vector<char>);
        std::vector<char> build_custom_echo(uint32_t dest_node);
        std::map<unsigned char,Route *> get_routing_table();

        std::map<unsigned char,Route *> myRoutingTable;
        std::map<unsigned char,bool> neighbors;

        std::vector<char> serialize_table(std::map<unsigned char,Route *>);
        uint32_t calculate_link_cost_from_rtt(const int RTT);
        std::map<unsigned char,Route *> process_payload(std::vector<char> payload);
        dynamic_addressing::DynamicAddressing THE_ADDRESSOR_20000;
        void print_table(std::map<unsigned char,Route *>);
        void print_interntal_table();
        void print_pkt_header(packet_header::Header pkt);
        packet_header::Header extract_header(std::vector<char> payload);
        void print_route(Route * r);
        void predict_next_hop(packet_header::Header * h);
        void start_ticking_thread();
        void start_broadcasting_thread();
        void put_neighbour_as_inactive(uint8_t i);
        void register_active_neighbour(uint8_t i);
        void inactive_neighbours_handling();


        unsigned int broadcast_to = BROADCAST_TIMEOUT; 

    private:
        unsigned int nodes_count = 4; // set it to 1 as we only know ourselve first
        BlockingQueue< Message >* senderQueue;
        void init_internal_table();





  
    };

    static void * tick(void * arg);
    static void * broadcast_table(void * args);

    struct thread_args {
        BlockingQueue< Message >* senderQueue;
        vector_routing_protocol::VectorRoutingProtocol* context;
    };


}
#endif