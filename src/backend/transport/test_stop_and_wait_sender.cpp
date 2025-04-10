
#include "../transport/StopAndWaitSender.h"
#include "../fragmentation/Fragmenter.h"
#include "../routing/VectorRoutingProtocol.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>

int main() {
    vector_routing_protocol::VectorRoutingProtocol routing;
    routing.THE_ADDRESSOR_20000.gen_random_addr();
    uint8_t my_addr = routing.THE_ADDRESSOR_20000.get_my_addr();
    uint8_t dest = 2;

    auto route = new vector_routing_protocol::Route();
    route->destination_node = dest;
    route->next_hop = dest;
    route->cost = 1;
    routing.myRoutingTable[dest] = route;

    StopAndWaitSender sender(&routing);
    sender.setSendFunction([](const std::vector<char>& packet) {
        std::cout << "[SEND] Packet size: " << packet.size() << std::endl;
    });

    std::string msg = "fragmentation test message";
    auto fragments = Fragmenter::fragmentMessage({msg.begin(), msg.end()}, my_addr, dest, dest, 42, 1);

    std::thread ack_thread([&]() {
        using namespace std::chrono_literals;
        for (size_t i = 0; i < fragments.size(); ++i) {
            std::this_thread::sleep_for(200ms);
            sender.handleAck(42, i);
        }
    });

    sender.sendFragments(fragments);
    ack_thread.join();
    std::cout << "StopAndWaitSender test passed\n";
    return 0;
}
