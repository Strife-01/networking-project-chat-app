#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "../../utils/packet_header.h"

class StopAndWaitSender {
public:
    StopAndWaitSender();

    void sendFragments(const std::vector<std::pair<packet_header::Header, std::vector<char>>>& fragments);

    void handleAck(uint8_t msg_id, uint8_t fragment_id);

private:
    void sendWithRetry(const packet_header::Header& header, const std::vector<char>& payload);

    std::atomic<bool> ackReceived;
    std::mutex ackMutex;
    std::condition_variable ackCond;

    uint8_t current_msg_id;
    uint16_t current_fragment_id;
};
