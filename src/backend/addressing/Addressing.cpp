#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "Addressing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;


/*


*/


namespace dynamic_addressing {


    DynamicAddressing::DynamicAddressing(){
        for(int i =1;i<=MAX_NODE_NUMBER;i++){
            this->addr_in_use[i] = false;
        }
    }


    void DynamicAddressing::gen_random_addr(){
        srandom(time(NULL));

        
        unsigned char tmp_addr = get_my_addr();
        // 50% chance to change its addr, as two nodes are concerned
        if( (rand()%100 <= 50) || (tmp_addr == 0) ){
            do{

                tmp_addr = rand() % MAX_NODE_NUMBER+1;

            }while(this->addr_in_use[tmp_addr] || tmp_addr == 0);
            
            printf("Randomly selected address : %d\n",tmp_addr);

            set_my_addr(tmp_addr);
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

    unsigned char DynamicAddressing::get_my_addr(){
        return dynamic_addressing::get_my_addr();
    }

    void DynamicAddressing::remove_addr_used_by_another_node(uint8_t addr){
        this->addr_in_use[addr] = false;
    }


}



unsigned char dynamic_addressing::get_my_addr(){

    // avoid two processes sharing the same address storage file 
    char file_name[20]; 
    sprintf(file_name,"my_addr_%d.net",getpid());

    FILE *f = fopen(file_name,"rb");
    unsigned char addr = 0;
    if(f != NULL){
        size_t bytes_read = fread(&addr,sizeof(char),1,f);
        fclose(f);
    }else{
        // create the file and put a bogus 0 value into it
        f = fopen(file_name,"ab+");
        unsigned char buff = 0;
        fwrite(&buff,sizeof(char),1, f);
        fclose(f);

    }
    return addr;

}

void dynamic_addressing::set_my_addr(unsigned char addr){

    // avoid two processes sharing the same address storage file 
    char file_name[20]; 
    sprintf(file_name,"my_addr_%d.net",getpid());
    
    FILE *f = fopen(file_name,"wb");

    if(f != NULL){
        fwrite(&addr, sizeof(char), 1, f);
    }
    fclose(f);
}

