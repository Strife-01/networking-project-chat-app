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
    for (int i = 0; i<= fragment_id; ++i, puts("")){
        for(int j=0;j<fs.fragments[i].size();j++){
            std::cout<< fs.fragments[i][j];
        }

    }
    
    if (!MF) {
        fs.total_fragments = fragment_id+1;
        fs.is_complete = true;
    }

    printf("total number of fragments received : %d\n",fs.total_fragments);
    if (fs.is_complete && fs.fragments.size() == fs.total_fragments) {
        std::vector<char> full;
        for (uint16_t i = 0; i < fs.total_fragments; ++ i) {
            printf("Inserting fragment %d\n",i);
            for(uint16_t j = 0;j<fs.fragments[i].size();j++){
                full.push_back(fs.fragments[i][j]);
            }
            //full.insert(full.end(), fs.fragments[i].begin(), fs.fragments[i].end());
        }
        buffers.erase(msg_id);
        return full;
    }
    return {};
}
