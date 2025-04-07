#ifndef INCLUDED_FRAGMENT
#define INCLUDED_FRAGMENT

#include <vector>
#include <cstdint>

struct Fragment {
    uint16_t messageID; // unique message id
    uint8_t fragmentIndex; // index of this fragment
    uint8_t totalFragments; // total numbers of totalFragments
    std::vector<char> payload; // actual data

    Fragment(uint16_t id, uint8_t index, uint8_t total, const std::vector<char>& data) : messageID(id), fragmentIndex(index), totalFragments(total), payload(data) {}
};

#endif
