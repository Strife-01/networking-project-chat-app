#include "VectorRoutingProtocol.h"
#include "Route.h"
#include <bitset>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <thread>
#include "../channel_state/ChannelState.h"
#include "../mac/MediumAccessControl.h"
#include "../../utils/Random.h"

namespace vector_routing_protocol {
    using namespace packet_header;

    VectorRoutingProtocol::VectorRoutingProtocol(BlockingQueue< Message >* sq) {


        senderQueue = sq;
        // first a foremost, to route something we need our own address :)
        THE_ADDRESSOR_20000 = dynamic_addressing::DynamicAddressing();
        // init routing table
        init_internal_table();

    }


    uint32_t VectorRoutingProtocol::calculate_link_cost_from_rtt(const int RTT){
        return 1;
    }


    std::map<unsigned char,Route *> VectorRoutingProtocol::process_payload(std::vector<char> payload){


        Header h = extract_header(payload);

        ////printf("%d\n\n",h.fields.dst_addr);

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


        return table;

    }

    std::vector<char> VectorRoutingProtocol::serialize_table(std::map<unsigned char,Route*> table){

        std::vector<char> payload;

        for(int i =1;i<=table.size();i++){
            payload.push_back(table[i]->destination_node);
            payload.push_back(table[i]->cost);
        }
        return payload;

    }



    void VectorRoutingProtocol::register_echo(std::vector<char> payload) {
 

        
        bool changes_have_been_made = false;

        Header h = extract_header(payload);

        //print_pkt_header(h);

        uint32_t src_node_addr = h.source_address;
        std::map<unsigned char,Route *> recv_routing_table = process_payload(payload);
        //print_table(recv_routing_table);


        uint8_t my_addr = THE_ADDRESSOR_20000.get_my_addr();

        // register myself only when sure i have generated the right address
        // to avoid having 1 (default addr) with a cost of zero in the table
        if((!first_table_received) || (wait_first_echo_recv_to <= 0) ){

            Route * r = myRoutingTable[my_addr];
            r->next_hop = my_addr;
            r->cost = 0;
            r->destination_node = my_addr;
            
        }




        //printf("received echo from %d\n",src_node_addr);

        unsigned char link_cost = calculate_link_cost_from_rtt(1);

        register_active_neighbour(src_node_addr);

        //printf("%d\n",src_node_addr);
        // check if the sender of the table is a new neighbour of us
        if(myRoutingTable[src_node_addr]->cost == INFINITY_COST || src_node_addr == my_addr){

            printf("Got a new neighbor !! Discovered node %d\n",src_node_addr);
            //puts("Checking if it does not share the same address as ours");

            if(src_node_addr == my_addr){
                
                handle_collision(my_addr);

            }else{
                //puts("Okay ! no collision, registering this new neighbour :D");
            }

            unsigned char potential_new_cost = link_cost;


            // insert newly discovered route in the table
            Route * r = myRoutingTable[src_node_addr];
            r->destination_node = src_node_addr;
            r->next_hop = src_node_addr;
            r->cost = potential_new_cost;
            r->TTL =MAX_TTL;

            changes_have_been_made = true;

        }
           

        // loop throught the table
        for(unsigned char i =1;i<=MAX_NODE_NUMBER;i++){


            if(my_addr == i) continue;

            // checking whether some destination is already in myRoutingTable, and accessing it:
            unsigned char dest_node = i;

            if(recv_routing_table[dest_node]->cost != INFINITY_COST){


                unsigned char potential_new_cost =recv_routing_table[dest_node]->cost+link_cost;

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
                            /*printf("Node %d is proposing a better cost to reach %d. ( %d vs %d )\n",
                                src_node_addr,
                                dest_node,
                                potential_new_cost,
                                myRoutingTable[dest_node]->cost
                            );*/

                            
                            changes_have_been_made = true;
                            myRoutingTable[dest_node]->cost = potential_new_cost;
                            myRoutingTable[dest_node]->next_hop = src_node_addr;
                            myRoutingTable[dest_node]->TTL =MAX_TTL;
                        }else{


                            // broadcast table to the other nodes
                            // if they have worst path than us
                            if((potential_new_cost > myRoutingTable[dest_node]->cost)

                            && (myRoutingTable[dest_node]->next_hop != src_node_addr)
                        
                            ){
                                changes_have_been_made = true;

                            }

                            // we are getting confirmation of our table
                            /*if((potential_new_cost == myRoutingTable[dest_node]->cost)

                            && (myRoutingTable[dest_node]->next_hop != src_node_addr)
                            ){
                                myRoutingTable
                            }*/
                              
                            /*if(
                                (myRoutingTable[dest_node]->cost == potential_new_cost)
                                 && (myRoutingTable[dest_node]->next_hop == src_node_addr)
                                ){
                                myRoutingTable[dest_node]->TTL = MAX_TTL;
                            }*/

                        }

                    }else{




                        if(dest_node != my_addr){
                            //printf("Got a new route !! Discovered node %d\n",dest_node);
                            // insert newly discovered route in the table
                            Route * r = myRoutingTable[src_node_addr];
                            r->next_hop = src_node_addr;
                            r->cost = potential_new_cost;
                            r->TTL =MAX_TTL;
                            //myRoutingTable[dest_node]=r;
                            changes_have_been_made = true;
                        }
                        
                        


                    }
                }else{
                    //myRoutingTable[dest_node].cost = INFINITY_COST;
                }

                
            }else{

                // remove if we get infinity from a node that was previously our next hop
                //printf("Got infinity cost to node %d from %d\n",dest_node,src_node_addr);


                if(
                (myRoutingTable[dest_node]->next_hop == src_node_addr) 
                && (dest_node != my_addr)
                ){

                    changes_have_been_made = true;
                    // advertise this path as broken
                    //printf("Broken link advertisment detected !!\n path to %d going throught %d\n",dest_node,src_node_addr);
                   // myRoutingTable[dest_node]->cost = INFINITY_COST;
                }

            }
            
            
        }


        // updating active neighbours table in the dynamic addressing handler

        
        if(changes_have_been_made){
            for(int i=1;i<=MAX_NODE_NUMBER;i++){

                // updating table of reachable nodes
                if((myRoutingTable[i]->cost != INFINITY_COST)){

                    register_active_node(i);

                    
                }else{
                    notify_unreachable_node(i);
                }
            }
            // do not broadcast here, first we fill the address table
            // then we generate the address if we have to
            // and finally we broadcast the table
        }


        THE_ADDRESSOR_20000.update_connected_nodes_list_from_RT(myRoutingTable);


        
        // allow address generation if not already made
        if(!first_table_received){
            first_table_received = true;
            // let time to the tick thread to tick
            this_thread::sleep_for(std::chrono::milliseconds(2500));
        }

        if(changes_have_been_made){
            puts("[+] Changes have been made, broadcasting table...");
            start_broadcasting_thread();
        }else{
            puts("[+] No changes have been made, not broadcasting table.");
            // add 5 to the broadcast timeout to delay new echo sending
            // and save bandwidth
            //this->broadcast_to += 1;
        }


        printf("[VARS] %d  ; %d\n",broadcast_to,!is_protocol_paused());


    }



    std::vector<char> VectorRoutingProtocol::build_custom_echo(uint32_t dest_node){


        packet_header::Header h;
        h.dest_address = dest_node;
        h.next_hop_address = dest_node;
        ////printf("Getting my address : %d\n",dynamic_addressing::get_my_addr());
        h.source_address = dynamic_addressing::get_my_addr();
        h.message_id = 1;
        h.fragment_id = 0;
        h.payload_length = 8;
        h.more_fragments = false;
        h.type = packet_header::echo;


        std::vector<char> payload = serialize_table(myRoutingTable);


        std::vector<char> serialized_header = packet_header::build_header(h);
  

        std::bitset<32> x(packet_header::bytes_vector_to_header_int(serialized_header));

        payload.insert(
            payload.begin(),
            serialized_header.begin(),
            serialized_header.end()
        );


        return payload;
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

                if(i == THE_ADDRESSOR_20000.get_my_addr()){
                    printf("\033[32m|  %d   |  %d   |  %d     |\033[0m\n",
                        table[i]->destination_node,
                        table[i]->cost,
                        table[i]->next_hop
                    );
                }else{
                    printf("|  %d   |  %d   |  %d     |\n",
                        table[i]->destination_node,
                        table[i]->cost,
                        table[i]->next_hop
                    );
                }

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


            // not needed anymore as address is 1 by default
            // and really determined only after a timeout
            // this function being called in constructor
            // the following chunk was not wishable anymore
            /*if(THE_ADDRESSOR_20000.get_my_addr() != i){
                r->cost = INFINITY_COST;
            }else{
                r->cost = 0;
            }*/
            r->cost = INFINITY_COST;
            r->next_hop = 0;
            r->TTL = MAX_TTL;
            myRoutingTable[i] = r;
            reachable_nodes[i] = false;
        }


    }

    void VectorRoutingProtocol::predict_next_hop(packet_header::Header * h){
        h->next_hop_address = myRoutingTable[h->dest_address]->next_hop;
    }

    static void * tick(void * args) {


        

        struct thread_args * ta = (struct thread_args *) args;

        //ta->context->THE_ADDRESSOR_20000.update_connected_nodes_list_from_RT(ta->context->myRoutingTable);

        bool first_run = true;

        while(true){


            if(
                (first_run && ta->context->first_table_received)
                 || (first_run && (ta->context->wait_first_echo_recv_to <=0))
        
            ){
                // generate random address from the first table we received
                // or if the timeout is triggered and we still did not receive anything
                puts("[+] first table received or the waiting timeout has been triggered.");
                ta->context->THE_ADDRESSOR_20000.gen_random_addr(true);
                first_run = false;
            }

            //Medium_Access_Control::mac_object.recalculate_wait_time();
            //int ttw = Random::get(0,1000);// + Medium_Access_Control::mac_object.get_wait_time();

            // broadcast only when we get an address
            //printf("[TICK] %d ; %d ; %d\n",ta->context->broadcast_to,!first_run,!ta->context->is_protocol_paused());
            if((ta->context->broadcast_to  <= 0) && (!first_run)  && (!ta->context->is_protocol_paused())){
                puts("BC TIMEOUT");
                ta->context->start_broadcasting_thread();
            }


            if(!ta->context->is_protocol_paused()){
                for(int i = 1;i<=MAX_NODE_NUMBER;i++){
                
                    if(ta->context->myRoutingTable[i]->TTL > 0){
                        ta->context->myRoutingTable[i]->TTL --;
                    }
    
                    if(ta->context->myRoutingTable[i]->TTL <= 0){
                        // if it was a neighbour, as the TTL expired,
                        // do not consider him as neighbour anymore
                        // in case of a topology change
                        ta->context->put_neighbour_as_inactive(i);
                        
    
                    }
                    // ta->context->neighbors[i] || first_run
                    // always checking myself to make sure no cost bug
    
                    if(!first_run){
                        if(i == dynamic_addressing::get_my_addr() ){
                            ta->context->myRoutingTable[i]->cost = 0;
                            continue;
                        }
                    }
       
                    //std::this_thread::sleep_for(std::chrono::milliseconds(ttw));
                }
    
                ta->context->inactive_neighbours_handling();

            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(TICK_TIME));
            
            // don't loop back to 2^31, it would be very unpleasant
            // do not decrease the timeout if the protocol is paused
            if((ta->context->broadcast_to > 0) && (!ta->context->is_protocol_paused())){
                ta->context->broadcast_to --;
            }
            if(first_run){
                ta->context->wait_first_echo_recv_to --;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        }    

        free(ta); // Clean up memory if you're done
        return NULL;
    }



    void VectorRoutingProtocol::inactive_neighbours_handling(){

        bool changes_have_been_made = false;
        for(int i = 1;i<=MAX_NODE_NUMBER;i++){

            if((neighbors[i] == false) && (myRoutingTable[i]->next_hop == i) && (i != THE_ADDRESSOR_20000.get_my_addr())){
                // INACTIVE NEIGBOR DETECTED
                //printf("Detected inactive neighbor : %d\n",i);

                // set the corresponding routing entry to infinity
                // for each routing entry that has this node as nextHop
                changes_have_been_made = true;
                for(uint32_t j=1;j<=MAX_NODE_NUMBER;j++){

                    if(myRoutingTable.count(j) > 0){
                        if(myRoutingTable[j]->next_hop == i){
                            myRoutingTable[j]->cost = INFINITY_COST;
                            myRoutingTable[j]->next_hop = 0;
                        }
                    }
                

                }
            }
                


            /*if(changes_have_been_made){
                start_broadcasting_thread();
            }*/


        }
    }


    void VectorRoutingProtocol::register_active_neighbour(uint8_t i){
        neighbors[i] = true;

    }

    void VectorRoutingProtocol::register_active_node(uint8_t i){
        //THE_ADDRESSOR_20000.register_addr_used_by_another_node(i);
        std::lock_guard<std::mutex> guard(mu_reachable_nodes);
        reachable_nodes[i] = true;
    }
    void VectorRoutingProtocol::notify_unreachable_node(uint8_t i){
        //THE_ADDRESSOR_20000.remove_addr_used_by_another_node(i);
        std::lock_guard<std::mutex> guard(mu_reachable_nodes);
        reachable_nodes[i] = false;
    }

    void VectorRoutingProtocol::put_neighbour_as_inactive(uint8_t i){
        ////printf("[+] Putting %d neighbour as inactive",i);
        THE_ADDRESSOR_20000.remove_addr_used_by_another_node(i);
        neighbors[i] = false;

    }
    
    void VectorRoutingProtocol::start_ticking_thread(){
        pthread_t ticking_thread_id;
        struct thread_args * args = (struct thread_args *) malloc(sizeof(struct thread_args));
    
        args->context = this;
        args->senderQueue = senderQueue;
        
    
        pthread_create(&ticking_thread_id, NULL, vector_routing_protocol::tick, args);
    }

    void VectorRoutingProtocol::start_broadcasting_thread(){

        // start a new thread if we don't already have an update pending
        // to not flood the network
        if(!update_broadcast_pending){
            pthread_t bc_thread_id;
            struct thread_args * args = (struct thread_args *) malloc(sizeof(struct thread_args));
        
            args->context = this;
            args->senderQueue = senderQueue;
            
            update_broadcast_pending = true;

            pthread_create(&bc_thread_id, NULL, vector_routing_protocol::broadcast_table, args);
        }else{
            broadcast_to = BCAST_TIMEOUT;
        }

    }



    static void * broadcast_table(void * args){

        
        vector_routing_protocol::thread_args * ta = (vector_routing_protocol::thread_args *)args;

        
        // reset broadcast TTL
        ta->context->broadcast_to = BCAST_TIMEOUT;


        Medium_Access_Control::mac_object.recalculate_wait_time();
        int ttw = Random::get(0,400);// + Medium_Access_Control::mac_object.get_wait_time();


        std::vector<char> packet = ta->context->build_custom_echo(0);

        ta->context->print_interntal_table();
        bool sent = false;
        while(!sent){
            ////printf("TRYING TO SEND");

            // as protocol could be paused during this separate thread
            if(!ta->context->is_protocol_paused()){
                if(!Channel_State::chan_state.get_is_line_busy())
                {
    
                    // sleep a random amount of time before sending
        
                    this_thread::sleep_for(chrono::milliseconds(ttw));
        
                    if(!Channel_State::chan_state.get_is_line_busy()
                
                    ){
                        Message sendMessage = Message(DATA, packet);
                        ta->senderQueue->push(sendMessage); // if this is what you want
                        puts("[i] Table update sent");
                        sent = true;
                        // wait extra random ms if we had the chance to send.
                        std::this_thread::sleep_for(std::chrono::milliseconds(Random::get(1000,2000)));
                        
                    }
        
                }else{
                    // update payload in case of change
                    packet = ta->context->build_custom_echo(0);
                }
            }else{
                sent = true;
            }


            
        }

        ta->context->update_broadcast_pending = false;


        pthread_exit(0);

    }


    void VectorRoutingProtocol::handle_collision(uint8_t other_node){

        if(first_table_received){
            puts("Oh NOOO ! Address collision. Starting recovery process...");
        
            THE_ADDRESSOR_20000.gen_random_addr(false);
    
            uint8_t new_addr = THE_ADDRESSOR_20000.get_my_addr();
    
    
            Route * r = myRoutingTable[new_addr];//(Route *) malloc(sizeof(Route *));
            r->destination_node = new_addr;
            r->next_hop = new_addr;
            r->cost = 0;
            r->TTL =MAX_TTL;
            myRoutingTable[new_addr]=r; 
        }

        
    }


    /*
    
    The two following methods are used
    to pause the protocol echos 
    to free bandwith if we send/forward messages
    */

    static void * resume_protocol_to(struct thread_args * ta){
        this_thread::sleep_for(std::chrono::minutes(PROTOCOL_PAUSE_TIMEOUT));

        VectorRoutingProtocol::resume_protocol();

        puts("[+] Protocol pause finished, resuming sending echo requests...");

        pthread_exit(0);
    }
    void VectorRoutingProtocol::pause_protocol(){
        pause_flag = true;

        thread_args * ta = (thread_args * )malloc(sizeof(thread_args));
        thread resume_to(resume_protocol_to,ta);
        resume_to.detach();
    }
    
    void VectorRoutingProtocol::resume_protocol(){
        pause_flag = false;
    }

    bool VectorRoutingProtocol::is_protocol_paused(){
        return pause_flag;
    }


    std::map<unsigned char,bool> get_reachable_nodes(){
        std::lock_guard<std::mutex> guard(mu_reachable_nodes);
        return reachable_nodes;
    }


    
}


    
