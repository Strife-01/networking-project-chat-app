#pragma once

#include <vector>
#include <functional>
#include "../transport/StopAndWaitSender.h"
#include "../transport/StopAndWaitReceiver.h"
#include "../fragmentation/Fragmenter.h"
#include "../routing/VectorRoutingProtocol.h"
#include "../../utils/packet_header.h"

class TransportManager {
public:
    TransportManager(vector_routing_protocol::VectorRoutingProtocol* routing);

    // network send function
    void setSendFunction(std::function<void(const std::vector<char>&)> sendFunc);

    // callback when a full message is received
    void setOnMessageReady(std::function<void(std::vector<char>)> callback);

    // send a full application-layer message to a destination
    void sendMessage(std::vector<char> message, uint8_t dest, uint8_t type = packet_header::types::data);

    // when a new packet arrives from the network
    void onPacketReceived(const std::vector<char>& packet);

    // forward ACKs from network
    void onAckReceived(std::vector<char> packet);

private:
    vector_routing_protocol::VectorRoutingProtocol* routing;
    StopAndWaitSender sender;
    StopAndWaitReceiver receiver;

    std::function<void(const std::vector<char>&)> sendFunc;
    std::function<void(std::vector<char>)> onMessageReady;

    uint8_t next_msg_id = 0;
};
