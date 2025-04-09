#include <cmath>
#include <cstdlib>
#include <time.h>
#include "Addressing.h"
#include <stdio.h>
using namespace std;


namespace dynamic_addressing {


    DynamicAddressing::DynamicAddressing(){
        for(int i =1;i<=MAX_NODE_NUMBER;i++){
            this->addr_in_use[i] = false;
        }
    }


    void DynamicAddressing::gen_random_addr(){
        srandom(time(NULL));
        
        // 50% chance to change its addr, as two nodes are concerned
        if( (rand()%100 <= 50) || (this->my_addr == 0) ){
            this->my_addr = rand() % MAX_NODE_NUMBER+1;
            do{
                this->my_addr = rand() % MAX_NODE_NUMBER+1;

            }while(this->addr_in_use[this->my_addr] || this->my_addr == 0);
            
            printf("Randomly selected address : %d\n",this->my_addr);
        }
    }

    void DynamicAddressing::update_connected_nodes_list_from_RT(std::map<unsigned char,vector_routing_protocol::Route *> rt){

        clear_nodes_addressing_map();
        
        for(int i=0;i<=MAX_NODE_NUMBER;i++){

            // if the node exists
            if(rt.count(i) > 0)
            {
                // if not ourselve or unreachable
                if( (rt[i]->cost != 0) && (rt[i]->cost < INFINITY_COST) ){
                    register_addr_used_by_another_node(i);
                }       
            }
 
        }


    }

    void DynamicAddressing::register_addr_used_by_another_node(unsigned char addr){
        this->addr_in_use[addr] = true;
    }

    void DynamicAddressing::clear_nodes_addressing_map(){
        for(int i=1;i<=MAX_NODE_NUMBER;i++){
            this->addr_in_use[i] = false;
        }
    }

}


