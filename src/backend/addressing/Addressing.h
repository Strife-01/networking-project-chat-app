#include <cstdint>
#include <vector>
#include <map>
#define MAX_NODE_NUMBER 4
#define CHANCE_TO_CHANGE 50
#include "../routing/Route.h"
#include <mutex>

#ifndef INFINITY_COST
    #define INFINITY_COST 5
#endif

#ifndef DYNADR
#define DYNADR
namespace dynamic_addressing {


    unsigned char get_my_addr();
    void set_my_addr(unsigned char);
    inline std::mutex mu_addr_file;
    
    class DynamicAddressing{
        public:
            DynamicAddressing();
            void gen_random_addr(bool init);
            void update_connected_nodes_list_from_RT(std::map<unsigned char,vector_routing_protocol::Route *> nodes);
            void register_addr_used_by_another_node(uint8_t addr);
            unsigned char get_my_addr();
            void remove_addr_used_by_another_node(uint8_t addr);
            

        private:
            std::map<unsigned char,bool> addr_in_use;
            void clear_nodes_addressing_map();

            unsigned char my_addr;
        
    };

}
#endif