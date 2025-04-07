namespace vector_routing_protocol {

    struct Route{
        uint32_t next_hop;
        uint32_t cost;
        uint32_t destination_node;
        uint32_t TTL;
    };

}