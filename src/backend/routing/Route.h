#ifndef ROUTE 
    #define ROUTE

    namespace vector_routing_protocol {

        typedef struct Route{
            unsigned char next_hop;
            unsigned char cost;
            unsigned char destination_node;
            unsigned char TTL;
        }Route;

    }

#endif