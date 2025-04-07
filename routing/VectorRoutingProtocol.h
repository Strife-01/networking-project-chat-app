

#include <cstdint>
#include <map>
#include "Route.h"

#define BROADCAST_ADDR 0
#define MAXIMUM_COST 4
#define INFINITY_COST 200000
#define MAX_NODE_NUMBER 4
#define MAX_TTL 1


namespace vector_routing_protocol {

    class VectorRoutingProtocol {
    public:
        VectorRoutingProtocol();
        void register_echo(char * payload);
        char * build_custom_echo(uint32_t dest_node);
        uint32_t my_address = 0;

    private:
        unsigned int nodes_count = 4; // set it to 1 as we only know ourselve first
        std::map<int32_t,int32_t> address_to_node_number;

        std::map<int32_t,struct Route *> myRoutingTable;
        std::map<int32_t,bool> neighbors;

        char * serialize_table(std::map<uint32_t,struct Route *>);
        uint32_t calculate_link_cost_from_rtt(const int RTT);
        std::map<uint32_t,struct Route *> process_payload(char * payload);
    };

}
