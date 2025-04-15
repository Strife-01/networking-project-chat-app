#pragma once

#include <map>
#include <vector>
#include <cstdint>

class Reassembler {
private:
    struct FragmentSet {
        std::map<uint16_t, std::vector<char>> fragments;
        bool is_complete = false;
        uint16_t total_fragments = 0;
        bool MF_recv = false;
    };

    std::map<uint8_t, FragmentSet> buffers;

public:
    std::vector<char> insertFragment(
        uint8_t msg_id,
        uint8_t fragment_id,
        bool MF,
        const std::vector<char>& payload
    );
};
