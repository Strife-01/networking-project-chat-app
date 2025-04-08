#include "Reassembler.h"
#include <cstdint>

std::vector<char> Reassembler::insertFragment(
        uint8_t msg_id,
        uint8_t fragment_id,
        bool MF,
        const std::vector<char>& payload
    ) {
    FragmentSet& fs = buffers[msg_id];
    fs.fragments[fragment_id] = payload;

    if (!MF) {
        fs.total_fragments = fragment_id + 1;
        fs.is_complete = true;
    }

    if (fs.is_complete && fs.fragments.size() == fs.total_fragments) {
        std::vector<char> full;
        for (uint16_t i = 0; i < fs.total_fragments; ++ i) {
            full.insert(full.end(), fs.fragments[i].begin(), fs.fragments[i].end());
        }
        buffers.erase(msg_id);
        return full;
    }
    return {};
}
