#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include "../routing/VectorRoutingProtocol.h"
#include "../../utils/packet_header.h"

#define MAX_BCAST_TRIES 5

class StopAndWaitSender {
public:
    StopAndWaitSender(vector_routing_protocol::VectorRoutingProtocol* routing);

    // function to send packets
    void setSendFunction(std::function<void(const std::vector<char>&)> func);

    // receive an ACK
    void handleAck(std::vector<char> packet);

    // send a sequence of fragments reliably
    void sendFragments(const std::vector<std::pair<packet_header::Header, std::vector<char>>>& fragments);
    std::function<void(const std::vector<char>&)> sendFunc;

private:
    vector_routing_protocol::VectorRoutingProtocol* routing;

    std::mutex ackMutex;
    std::condition_variable ackCond;
    std::atomic<bool> ackReceived;

    uint8_t current_msg_id;
    uint16_t current_fragment_id;

    void sendWithRetry(packet_header::Header header, const std::vector<char>& payload);

    static void sendBroadcast(StopAndWaitSender * sender_obj,packet_header::Header header,const std::vector<char>&payload);
};
