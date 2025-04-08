#include "VectorRoutingProtocol.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>


namespace vector_routing_protocol {

    VectorRoutingProtocol::VectorRoutingProtocol() {

        // first a foremost, to route something we need our own address :)
        THE_ADDRESSOR_20000 = dynamic_addressing::DynamicAddressing();
        THE_ADDRESSOR_20000.gen_random_addr();
    }


    uint32_t VectorRoutingProtocol::calculate_link_cost_from_rtt(const int RTT){
        return 1;
    }


    std::map<uint32_t,Route *> VectorRoutingProtocol::process_payload(char * payload){

        packet_header h;
        h.header = ((unsigned int*) payload)[0];

        uint32_t payload_len = h.fields.payload_len;
        uint32_t src_node_addr = h.fields.src_addr;
        char * serialized_table = (char *) ( ((unsigned int*) payload)+1 );


        std::map<uint32_t,Route *> table;
        
        // loop through the table

        for(int i = 0;i<payload_len;i = i+2){

            Route * r = (Route *) malloc(sizeof(Route));

            r->next_hop = src_node_addr;
            r->destination_node = ((uint32_t *) serialized_table)[i];
            r->cost = ((uint32_t *) serialized_table)[i+1];
            table[((uint32_t *) serialized_table)[i]] = r;
        }


        return table;

    }

    std::vector<char> VectorRoutingProtocol::serialize_table(std::map<uint32_t,Route*> table){

        std::vector<char> payload;

        for(int i =0;i<table.size();i++){

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



    void VectorRoutingProtocol::register_echo(std::vector<char> shitty_format_payload) {
 

        char * payload = shitty_format_payload.data();
        
        packet_header h;
        h.header = ((unsigned int * ) payload)[0];

        uint32_t src_node_addr = h.fields.src_addr;
        std::map<uint32_t,Route *> recv_routing_table = process_payload(payload);
        

        // myself
        if(myRoutingTable.count(THE_ADDRESSOR_20000.my_addr) <= 0){
            Route * r = (Route *) malloc(sizeof(Route *));
            r->next_hop = THE_ADDRESSOR_20000.my_addr;
            r->cost = 0;
            r->destination_node = THE_ADDRESSOR_20000.my_addr;
            myRoutingTable[THE_ADDRESSOR_20000.my_addr]=r; 
        }


        // reset neigboring nodes activity
        for(int i = 1;i<=MAX_NODE_NUMBER;i++){
            if(neighbors.count(i)){
                neighbors[i] = false;
            }
    
        }


        std::cout<< "received echo from " << src_node_addr << " with "
        << recv_routing_table.size() << " rows"<<std::endl;

        uint32_t link_cost = calculate_link_cost_from_rtt(1);

        for (int i = 0; i < recv_routing_table.size(); i++) {

             // we will use the node address as node index in the table
            if(this->neighbors.count(src_node_addr) <= 0){

                printf("Got a new neighbor !! Discovered node %d\n",src_node_addr);
                printf("Checking if it does not share the same address as ours\n");

                if(src_node_addr == THE_ADDRESSOR_20000.my_addr){
                    printf("Oh NOOO ! Address collision. Starting recovery process...");
                    
                    THE_ADDRESSOR_20000.gen_random_addr();
                }else{
                    // still, new node means we have to register it
                    THE_ADDRESSOR_20000.register_addr_used_by_another_node(src_node_addr);
                }


                unsigned int potential_new_cost = link_cost;
                // insert newly discovered route in the table
                Route * r = (Route *) malloc(sizeof(Route *));
                r->next_hop = src_node_addr;
                r->cost = potential_new_cost;
                r->TTL =MAX_TTL;
                myRoutingTable[src_node_addr]=r;                
            }
            neighbors[src_node_addr] = true;

            
            
           

            for(int32_t i =1;i<=this->nodes_count;i++){


                // checking whether some destination is already in myRoutingTable, and accessing it:
                int32_t dest_node = i;
                

                if(recv_routing_table[dest_node]->cost != INFINITY_COST){

                    unsigned int potential_new_cost =recv_routing_table[dest_node]->cost+link_cost;

                    // barrier against infinite incrementation
                    if(!(potential_new_cost > MAXIMUM_COST)){
                        // already a route for it ?
                        if (myRoutingTable.count(dest_node)) {
                            //printf("Already found an entry for node %d in table\n",dest_node);
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
                            }

                        }else{

                            if(i != THE_ADDRESSOR_20000.my_addr){
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

        THE_ADDRESSOR_20000.update_connected_nodes_list_from_RT(myRoutingTable)
        

        printf("Transmitting data table to new nodes !\n");


    
        
    }



    // resetting do not update table
    std::vector<char> VectorRoutingProtocol::build_custom_echo(uint32_t dest_node){

        std::map<uint32_t,Route*> tmp_routing_table;

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
                    if( (myRoutingTable[i]->cost == 0) && (i != THE_ADDRESSOR_20000.my_addr) ){
                        r->cost = INFINITY_COST;
                    }

                }

            }else{
                // path to ourselve
                if(i == THE_ADDRESSOR_20000.my_addr){
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

        return serialize_table(tmp_routing_table);

    }


    std::map<int32_t,Route *> VectorRoutingProtocol::get_routing_table(){
        return myRoutingTable;
    }
    
}


    
