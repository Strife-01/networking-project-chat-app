#include "Fragmenter.h"
#include <cstddef>
#include <cstdint>

std::vector<std::pair<packet_header, std::vector<char>>> Fragmenter::fragmentMessage(
    const std::vector<char>& message,
    uint8_t src, uint8_t dst, uint8_t nextHop, uint8_t msg_id, uint8_t type
) {
    std::vector<std::pair<packet_header, std::vector<char>>> fragments;

    std::size_t headerSize = size_of(packet_header);
    std::size_t maxPayloadSize = 32 - headerSize;
    std::size_t totalFragments = (message.size() + maxPayloadSize - 1) / maxPayloadSize;

    for (std::size_t i = 0; i < totalFragments; ++ i) {
        size_t start = i * maxPayloadSize;
        size_t end = std::min(start + maxPayloadSize, message.size());
        std::vector<char> chunk(message.begin() + start, message.begin() + end);

        packet_header h;
        h.fields.payload_len = chunk.size();
        h.fields.type = type;
        h.fields.fragment_id = i;
        h.fields.msg_id = msg_id;
        h.fields.MF = (i < totalFragments - 1);
        h.fields.dst_addr = dst;
        h.fields.next_hop = nextHop;
        h.fields.src_addr = src;

        fragments.emplace_back(h, chunk);
    }
    return fragments;
}

