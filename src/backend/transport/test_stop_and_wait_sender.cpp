#include "../fragmentation/Fragmenter.h"
#include "StopAndWaitSender.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::string msg = "this is a test message - try to see if this is working";

    std::vector<char> data(msg.begin(), msg.end());

    uint8_t src = 1, dst = 2, hop = 2, id = 42, type = 1;
    auto fragments = Fragmenter::fragmentMessage(data, src, dst, hop, id, type);

    StopAndWaitSender sender;

    std::thread ackThread([&]() {
        using namespace std::chrono_literals;
        for (size_t i = 0; i < fragments.size(); ++ i) {
            std::this_thread::sleep_for(100ms);
            sender.handleAck(id, i);
        }
    });

    sender.sendFragments(fragments);
    ackThread.join();
}
