#include <vector>
#include <map>
#define MAX_NODE_NUMBER 4
#define CHANCE_TO_CHANGE 50
#include "../routing/Route.h"

#ifndef INFINITY_COST
    #define INFINITY_COST 5
#endif


namespace dynamic_addressing {
    class DynamicAddressing{
        public:
            DynamicAddressing();
            unsigned char my_addr = 0;
            void gen_random_addr();
            void update_connected_nodes_list_from_RT(std::map<unsigned char,vector_routing_protocol::Route *> nodes);
            void register_addr_used_by_another_node(unsigned char addr);

        private:
            std::map<unsigned char,bool> addr_in_use;
            void clear_nodes_addressing_map();
        
    };

}
