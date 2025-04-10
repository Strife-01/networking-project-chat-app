#pragma once

#include <vector>
#include <functional>
#include "../routing/VectorRoutingProtocol.h"
#include "../fragmentation/Reassembler.h"
#include "../multihop-relay/MultihopRelay.h"
#include "../../utils/packet_header.h"

class StopAndWaitReceiver {
public:
    StopAndWaitReceiver(vector_routing_protocol::VectorRoutingProtocol* routing);

    void setSendFunction(std::function<void(const std::vector<char>&)> callback);
    void setOnMessageReady(std::function<void(std::vector<char>)> callback);

    void onPacketReceived(const std::vector<char>& packet);

private:
    vector_routing_protocol::VectorRoutingProtocol* routing;
    Reassembler reassembler;
    multihop_relay::MultihopRelay relay;

    std::function<void(std::vector<char>)> sendFunc;
    std::function<void(std::vector<char>)> onMessageReady;

    void sendAck(uint8_t msg_id, uint16_t frag_id, uint8_t dest);
};
