#pragma once

#include <vector>
#include <cstdint>
#include "../../utils/packet_header.h"

class Fragmenter {
public:
    static std::vector<std::pair<packet_header::Header, std::vector<char>>> fragmentMessage(
        const std::vector<char>& message,
        uint8_t src, uint8_t dst, uint8_t nextHop, uint8_t message_id, uint8_t type
    );
};
