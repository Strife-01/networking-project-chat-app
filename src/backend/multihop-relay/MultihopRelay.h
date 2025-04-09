#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "../../utils/packet_header.h"

namespace multihop_relay {

    class MultihopRelay{

        public:
            MultihopRelay();
            bool is_packet_to_forward(std::vector<char> packet);
            std::vector<char> prepare_header_to_forward(std::vector<char> packet,std::map<unsigned char,vector_routing_protocol::Route *> table);
        private:
    };
}