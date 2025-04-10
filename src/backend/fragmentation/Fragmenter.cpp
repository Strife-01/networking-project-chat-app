#include "Fragmenter.h"
#include <cstddef>
#include <cstdint>

std::vector<std::pair<packet_header::Header, std::vector<char>>> Fragmenter::fragmentMessage(
    const std::vector<char>& message,
    uint8_t src, uint8_t dst, uint8_t nextHop, uint8_t msg_id, uint8_t type
) {
    std::vector<std::pair<packet_header::Header, std::vector<char>>> fragments;

    std::size_t headerSize = 4;
    std::size_t maxPayloadSize = 32 - headerSize;
    std::size_t totalFragments = (message.size() + maxPayloadSize - 1) / maxPayloadSize;

    for (std::size_t i = 0; i < totalFragments; ++ i) {
        size_t start = i * maxPayloadSize;
        size_t end = std::min(start + maxPayloadSize, message.size());
        std::vector<char> chunk(message.begin() + start, message.begin() + end);

        packet_header::Header h;
        h.payload_length = chunk.size();
        h.type = type;
        h.fragment_id = i;
        h.message_id = msg_id;
        h.more_fragments = (i < totalFragments - 1);
        h.dest_address = dst;
        h.next_hop_address = nextHop;
        h.source_address = src;

        fragments.emplace_back(h, chunk);
    }
    return fragments;
}

