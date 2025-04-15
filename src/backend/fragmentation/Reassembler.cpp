#include "Reassembler.h"
#include <cstdint>
#include <stdio.h>
#include <iostream>

// BUFFERS FRAGMENTS, WAITS UNTIL ALL ARE ARRIVED

// check for duplicate fragments
std::vector<char> Reassembler::insertFragment(
        uint8_t msg_id,
        uint8_t fragment_id,
        bool MF,
        const std::vector<char>& payload
    ) {
    FragmentSet& fs = buffers[msg_id];
    fs.fragments[fragment_id] = payload;

    puts("buffer content :");
    uint16_t last_fragment_id = 0; // only if one no-mf fragment has been recv
    for (int i = 0; i<= fragment_id; ++i, puts("")){

        for(int j=0;j<fs.fragments[i].size();j++){
            std::cout<< fs.fragments[i][j];
        }

    }
    
    fs.is_complete = true;

    if (!MF) {
        fs.total_fragments = fragment_id+1;
        fs.MF_recv = true;
    }

    for(int i =0;i<fs.total_fragments;i++){
        if(fs.fragments.count(i) > 0){
            fs.is_complete = fs.is_complete && !fs.fragments[i].empty();
        }else{
            fs.is_complete = false;
        }
    }



    printf("[+] Total number of fragments received : %zu\n",fs.fragments.size());
    printf("\t[i] Last fragment received : %d\n",fragment_id);
    printf("\t[i] Packet complete : %d\n",fs.is_complete);
    if (fs.is_complete && fs.MF_recv) {
        std::vector<char> full;
        for (uint16_t i = 0; i < fs.total_fragments; ++ i) {
            printf("Inserting fragment %d\n",i);
            for(uint16_t j = 0;j<fs.fragments[i].size();j++){
                full.push_back(fs.fragments[i][j]);
            }
            //full.insert(full.end(), fs.fragments[i].begin(), fs.fragments[i].end());
        }
        buffers.erase(msg_id);
        fs.is_complete = false;
        return full;
    }

    return {};
}
