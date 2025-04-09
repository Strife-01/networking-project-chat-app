#include "VectorRoutingProtocol.h"
#include "Route.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <thread>


namespace vector_routing_protocol {
    using namespace packet_header;

    VectorRoutingProtocol::VectorRoutingProtocol() {

        // first a foremost, to route something we need our own address :)
        THE_ADDRESSOR_20000 = dynamic_addressing::DynamicAddressing();
        THE_ADDRESSOR_20000.gen_random_addr();

        // init routing table
        init_internal_table();
    }


    uint32_t VectorRoutingProtocol::calculate_link_cost_from_rtt(const int RTT){
        return 1;
    }


    std::map<unsigned char,Route *> VectorRoutingProtocol::process_payload(std::vector<char> payload){


        Header h = extract_header(payload);

        //printf("%d\n\n",h.fields.dst_addr);
        // TODO: fix types

        uint8_t payload_len = h.payload_length;
        uint8_t src_node_addr = h.source_address;
        std::vector<char> serialized_table(payload.begin()+4,payload.end());
        

        std::map<unsigned char,Route *> table;
        
        // loop through the table

        for(int i = 0;i<payload_len;i = i+2){

            Route * r = (Route *) malloc(sizeof(Route));
            
            r->next_hop = src_node_addr;
            r->destination_node = serialized_table[i];
            r->cost = serialized_table[i+1];
            table[serialized_table[i]] = r;
        }

        printf("table size %zu\n",table.size());


        return table;

    }

    std::vector<char> VectorRoutingProtocol::serialize_table(std::map<unsigned char,Route*> table){

        std::vector<char> payload;

        for(int i =1;i<=table.size();i++){

            payload.push_back(table[i]->destination_node);
            payload.push_back(table[i]->cost);
            /*if(payload.empty()){
                payload = (unsigned char * ) malloc(sizeof(uint32_t)*2);
                
                ((uint32_t *)payload)[0] = table[i]->destination_node;
                ((uint32_t *)payload)[1] = table[i]->cost;
                payload.insert(table[i]->destination_node);
                payload.insert(table[i]->cost)
            }else{
                payload = (unsigned char *) realloc(payload,sizeof(uint32_t)*2*i);
                ((uint32_t *)payload)[i] = table[i]->destination_node;
                ((uint32_t *)payload)[i+1] = table[i]->cost;        
            }*/
        }
        return payload;

    }



    void VectorRoutingProtocol::register_echo(std::vector<char> payload) {
 
        
        Header h = extract_header(payload);

        print_pkt_header(h);

        uint32_t src_node_addr = h.source_address;
        std::map<unsigned char,Route *> recv_routing_table = process_payload(payload);
        print_table(recv_routing_table);



        // myself
        if(myRoutingTable.count(THE_ADDRESSOR_20000.get_my_addr()) <= 0){
            Route * r = (Route *) malloc(sizeof(Route *));
            r->next_hop = THE_ADDRESSOR_20000.get_my_addr();
            r->cost = 0;
            r->destination_node = THE_ADDRESSOR_20000.get_my_addr();
            myRoutingTable[THE_ADDRESSOR_20000.get_my_addr()]=r; 
        }


        // reset neigboring nodes activity
        for(int i = 1;i<=MAX_NODE_NUMBER;i++){
            if(neighbors.count(i)){
                neighbors[i] = false;
            }
    
        }


        printf("received echo from %d\n",src_node_addr);

        unsigned char link_cost = calculate_link_cost_from_rtt(1);


        // check if the sender of the table is a new neighbour of us
        if(this->neighbors.count(src_node_addr) <= 0){

            printf("Got a new neighbor !! Discovered node %d\n",src_node_addr);
            puts("Checking if it does not share the same address as ours");

            if(src_node_addr == THE_ADDRESSOR_20000.get_my_addr()){
                puts("Oh NOOO ! Address collision. Starting recovery process...");
                THE_ADDRESSOR_20000.gen_random_addr();
                // if we kept the same address, let it stay at cost 0
                if(src_node_addr == THE_ADDRESSOR_20000.get_my_addr())
                    link_cost = 0;

            }else{
                puts("Okay ! no collision, registering this new neighbour :D");
                // still, new node means we have to register it
                THE_ADDRESSOR_20000.register_addr_used_by_another_node(src_node_addr);
            }

            unsigned char potential_new_cost = link_cost;


            // insert newly discovered route in the table
            Route * r = (Route *) malloc(sizeof(Route *));
            r->next_hop = src_node_addr;
            r->cost = potential_new_cost;
            r->TTL =MAX_TTL;
            myRoutingTable[src_node_addr]=r; 

            
        }
        neighbors[src_node_addr] = true;
           

        // loop throught the table
        for(unsigned char i =1;i<=MAX_NODE_NUMBER;i++){



            // checking whether some destination is already in myRoutingTable, and accessing it:
            unsigned char dest_node = i;
            
            if(recv_routing_table[dest_node]->cost != INFINITY_COST){

                unsigned char potential_new_cost =recv_routing_table[dest_node]->cost+link_cost;

                // barrier against infinite incrementation
                if(!(potential_new_cost > MAXIMUM_COST)){
                    // already a route for it ?
                    if (myRoutingTable.count(dest_node)) {
                        printf("Already found an entry for node %d in table\n",dest_node);
                        // now access the Route as   myRoutingTable[somedest_node]
                        if(
                            (myRoutingTable[dest_node]->cost > potential_new_cost)
                            ||
                            (myRoutingTable[dest_node]->cost == INFINITY_COST)
                            ||
                            (myRoutingTable[dest_node]->TTL <= 0)
                        ){
                            printf("Node %d is proposing a better cost to reach %d. ( %d vs %d )\n",
                                src_node_addr,
                                dest_node,
                                potential_new_cost,
                                myRoutingTable[dest_node]->cost
                            );
                            
                            myRoutingTable[dest_node]->cost = potential_new_cost;
                            myRoutingTable[dest_node]->next_hop = src_node_addr;
                            myRoutingTable[dest_node]->TTL =MAX_TTL;
                        }else{

                            printf("Incoming table did not propose a better path to node %d\n",dest_node);
                            // last if do not check the case of non changing cost
                            // but in that case, we don't want a new route,
                            // just to reset TTL
                            if(myRoutingTable[dest_node]->cost == potential_new_cost){
                                myRoutingTable[dest_node]->TTL = MAX_TTL;
                            }

                        }

                    }else{




                        if(dest_node != THE_ADDRESSOR_20000.get_my_addr()){
                            printf("Got a new route !! Discovered node %d\n",dest_node);
                            // insert newly discovered route in the table
                            Route * r = (Route *) malloc(sizeof(Route *));
                            r->next_hop = src_node_addr;
                            r->cost = potential_new_cost;
                            r->TTL =MAX_TTL;
                            myRoutingTable[dest_node]=r;
                        }
                        
                        


                    }
                }else{
                    //myRoutingTable[dest_node].cost = INFINITY_COST;
                }

                
            }else{

                // remove if we get infinity from a node that was previously our next hop
                printf("Got infinity cost to node %d from %d\n",dest_node,src_node_addr);


                if(
                (myRoutingTable.count(dest_node) > 0)
                && 
                (myRoutingTable[dest_node]->next_hop == src_node_addr)
                ){
                    // advertise this path as broken
                    printf("Broken link advertisment detected !!\n path to %d going throught %d\n",dest_node,src_node_addr);
                    myRoutingTable[dest_node]->cost = INFINITY_COST;
                }

            }
            
            
        }

        


        // check inactive neighboors ( that did not send a packet)
        for(int i = 1;i<=MAX_NODE_NUMBER;i++){


            if((neighbors.count(i) > 0 ) ){

                if(neighbors[i] == false){
                    // INACTIVE NEIGBOR DETECTED

                    printf("Detected inactive neighbor : %d\n",i);

                    // set the corresponding routing entry to infinity
                    // for each routing entry that has this node as nextHop

                    for(uint32_t j=1;j<=MAX_NODE_NUMBER;j++){

                        if(myRoutingTable.count(j) > 0){
                            if(myRoutingTable[j]->next_hop == i){
                                myRoutingTable[j]->cost = INFINITY_COST;
                            }
                        }
                        
                        // advertise the other neighbors for this broken link
                        // will be done by sending our table

                    }

                    // remove this neighbor from the neighbor table 
                    neighbors.erase(i);
                }
                


            }


        }



        // updating active neighbours table in the dynamic addressing handler

        THE_ADDRESSOR_20000.update_connected_nodes_list_from_RT(myRoutingTable);
        

        printf("Transmitting data table to new nodes !\n");


    
        
    }



    // resetting do not update table
    std::vector<char> VectorRoutingProtocol::build_custom_echo(uint32_t dest_node){

        std::map<unsigned char,Route*> tmp_routing_table;

        // split horizon poison reverse
        for(int i=1;i<=MAX_NODE_NUMBER;i++){



            Route * r  = (Route *) malloc(sizeof(Route));
            *r = *myRoutingTable[i];

            if(myRoutingTable.count(i)){


                // if we are trying to send a packet to 
                // a node that has one of our route passing throught it
                // set this route cost to infinity
                if(myRoutingTable[i]->next_hop == i){
                    r->cost = INFINITY_COST;
                }else{

                    // somehow we got a table full of 0 at the very start of the alrgorithm
                    // We didn't manage to find WHY ????????????????????
                    // so, here we are, doing some patch-up job x'(
                    if( (myRoutingTable[i]->cost == 0) && (i != THE_ADDRESSOR_20000.get_my_addr()) ){
                        r->cost = INFINITY_COST;
                    }

                }

            }else{
                // path to ourselve
                if(i == THE_ADDRESSOR_20000.get_my_addr()){
                    r->cost = 0;
                }else{
                    // node we don't know yet ( we know it exists because of the rule of 6)
                    r->cost = INFINITY_COST;
                }
            }
            tmp_routing_table[i] = r;

                    
                

                // pointless to send to non neighbours but well, it's more readable
                // decrements TTL
                myRoutingTable[i]->TTL --;

        }

        packet_header::Header h;
        h.dest_address = dest_node;
        h.next_hop_address = dest_node;
        h.source_address = dynamic_addressing::get_my_addr();
        h.message_id = 1;
        h.fragment_id = 0;
        h.payload_length = 8;
        h.type = packet_header::types(echo);
        return packet_header::add_header_to_payload(h, serialize_table(tmp_routing_table));

    }


    std::map<unsigned char,Route *> VectorRoutingProtocol::get_routing_table(){
        return myRoutingTable;
    }


    void VectorRoutingProtocol::print_interntal_table(){
        print_table(myRoutingTable);
    }

    void VectorRoutingProtocol::print_table(std::map<unsigned char,Route *> table){
        
        puts("| NODE | COST | NXTHOP |");
        puts("========================");
        for(int i=1;i<=table.size();i++){
            if(table.count(i)){
                printf("|  %d   |  %d   |  %d     |\n",
                    table[i]->destination_node,
                    table[i]->cost,
                    table[i]->next_hop
                );
            }

        }
        puts("========================");
    }

    void VectorRoutingProtocol::print_pkt_header(Header pkt){
        std::cout << "Packet Header Content:\n";
        std::cout << "----------------------\n";
        std::cout << std::setw(20) << std::left << "Payload Length:" << static_cast<int>(pkt.payload_length) << "\n";
        std::cout << std::setw(20) << std::left << "Type:" << static_cast<int>(pkt.type) << "\n";
        std::cout << std::setw(20) << std::left << "Fragment ID:" << pkt.fragment_id << "\n";
        std::cout << std::setw(20) << std::left << "Message ID:" << static_cast<int>(pkt.message_id) << "\n";
        std::cout << std::setw(20) << std::left << "More Fragments (MF):" << std::boolalpha << pkt.more_fragments << "\n";
        std::cout << std::setw(20) << std::left << "Destination Address:" << static_cast<int>(pkt.dest_address) << "\n";
        std::cout << std::setw(20) << std::left << "Next Hop:" << static_cast<int>(pkt.next_hop_address) << "\n";
        std::cout << std::setw(20) << std::left << "Source Address:" << static_cast<int>(pkt.source_address) << "\n";
        std::cout << "----------------------\n";
    }

    Header VectorRoutingProtocol::extract_header(std::vector<char> payload){
        uint32_t header_int = bytes_vector_to_header_int(payload);
        Header h = get_separated_header(header_int);
        return h;
    }

    void VectorRoutingProtocol::print_route(Route * r){
        printf("destination : %d | next_hop : %d | cost %d | TTL %d\n",
            r->destination_node,
            r->next_hop,
            r->cost,
            r->TTL
        );
    }

    void VectorRoutingProtocol::init_internal_table(){

        for(int i =1;i<=MAX_NODE_NUMBER;i++){
            Route * r = (Route *) malloc(sizeof(Route));
            r->destination_node = i;


            if(THE_ADDRESSOR_20000.get_my_addr() != i){
                r->cost = INFINITY_COST;
            }else{
                r->cost = 0;
            }


            r->next_hop = 0;
            r->TTL = MAX_TTL;
            myRoutingTable[i] = r;
        }

    }

    void VectorRoutingProtocol::predict_next_hop(packet_header::Header * h){
        h->next_hop_address = myRoutingTable[h->dest_address]->next_hop;
    }

    static void * tick(void * args) {
        struct thread_args * ta = (struct thread_args *) args;
    
        for(int i = 1;i<=MAX_NODE_NUMBER;i++){
            if(ta->context->neighbors[i]){
                std::vector<char> packet = ta->context->build_custom_echo(i);
                Message sendMessage = Message(DATA, packet);
                ta->senderQueue->push(sendMessage); // if this is what you want
            }
        
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    
        free(ta); // Clean up memory if you're done
        return NULL;
    }
    
    void VectorRoutingProtocol::start_thread(BlockingQueue< Message >* senderQueue){
        pthread_t ticking_thread_id;
        struct thread_args * args = (struct thread_args *) malloc(sizeof(struct thread_args));
    
        args->context = this;
        args->senderQueue = senderQueue;
    
        pthread_create(&ticking_thread_id, NULL, VectorRoutingProtocol::tick, args);
    }

 


    
}


    
