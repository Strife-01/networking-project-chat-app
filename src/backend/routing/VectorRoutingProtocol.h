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
#include <mutex>

#define BROADCAST_ADDR 0
#define MAXIMUM_COST 7
#define INFINITY_COST 5
#define MAX_NODE_NUMBER 4
#define MAX_TTL 40
#define BCAST_TIMEOUT 10
#define WAIT_FIRST_ECHO_RECV_TIMEOUT 30
#define MEAN_RTT 100 // implement a real computation later
#define TICK_TIME 1000
#define PROTOCOL_PAUSE_TIMEOUT 80
#define MAX_BCAST_TIMEOUT 50


namespace vector_routing_protocol {

    inline std::map<unsigned char,bool> neighbors;
    inline std::map<unsigned char,bool> reachable_nodes;
    inline bool pause_flag = false;
    inline std::mutex mu_reachable_nodes;

    class VectorRoutingProtocol {
    public:
        VectorRoutingProtocol(BlockingQueue< Message >* senderQueue);
        void register_echo(std::vector<char>);
        std::vector<char> build_custom_echo(uint32_t dest_node);
        std::map<unsigned char,Route *> get_routing_table();

        std::map<unsigned char,Route *> myRoutingTable;
        //std::map<unsigned char,bool> neighbors;

        std::vector<char> serialize_table(std::map<unsigned char,Route *>);
        uint32_t calculate_link_cost_from_rtt(const int RTT);
        std::map<unsigned char,Route *> process_payload(std::vector<char> payload);
        dynamic_addressing::DynamicAddressing THE_ADDRESSOR_20000;
        void print_table(std::map<unsigned char,Route *>);
        void print_interntal_table();
        static void print_pkt_header(packet_header::Header pkt);
        packet_header::Header extract_header(std::vector<char> payload);
        void print_route(Route * r);
        void predict_next_hop(packet_header::Header * h);
        void start_ticking_thread();
        void start_broadcasting_thread();
        void put_neighbour_as_inactive(uint8_t i);
        void register_active_neighbour(uint8_t i);
        void inactive_neighbours_handling();
        void notify_unreachable_node(uint8_t i);
        void register_active_node(uint8_t i);

        unsigned int broadcast_to = BCAST_TIMEOUT;
        bool first_table_received = false;

        unsigned int wait_first_echo_recv_to = WAIT_FIRST_ECHO_RECV_TIMEOUT;


        static void pause_protocol();
        static void resume_protocol();
        static bool is_protocol_paused();

        bool update_broadcast_pending = false;

    private:
        unsigned int nodes_count = 4; // set it to 1 as we only know ourselve first
        BlockingQueue< Message >* senderQueue;
        void init_internal_table();
        void handle_collision(uint8_t other_node);

        uint16_t current_bcast_timeout_max_value = BCAST_TIMEOUT;



  
    };

    static void * tick(void * arg);
    static void * broadcast_table(void * args);

    std::map<unsigned char,bool> get_reachable_nodes();

    struct thread_args {
        BlockingQueue< Message >* senderQueue;
        vector_routing_protocol::VectorRoutingProtocol* context;
    };


}
#endif
