#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>
#include "../../utils/test_framework.h"

#include "Fragmenter.h"
#include "Reassembler.h"

uint8_t generate_msg_id() {
    static uint8_t counter = 0;
    return counter ++;
}

static std::vector<std::pair<packet_header::Header, std::vector<char>>> fragmentMessage (std::string message) {
    std::string input = message;

    std::vector<char> original_msg(input.begin(), input.end());

    uint8_t src = 1;
    uint8_t dst = 2;
    uint8_t nextHop = 3;
    uint8_t msg_id = generate_msg_id();
    uint8_t type = 1;

    auto fragments = Fragmenter::fragmentMessage(original_msg, src, dst, nextHop, msg_id, type);

    std::cout << "[TEST] Total nmber of fragments: " << fragments.size() << std::endl;

    return fragments;
}

std::string test_reassembly(std::vector<std::pair<packet_header::Header, std::vector<char>>> fragments) {
    Reassembler reassembler;

    std::vector<char> result;
    for (const auto& [header, payload] : fragments) {
        result = reassembler.insertFragment(
            header.message_id,
            header.fragment_id,
            header.more_fragments,
            payload
        );
    }
    std::string output(result.begin(), result.end());

    std:: cout << "[TEST] The reassembled message: " << output << "\n";

    return output;
}


// to run this from the root dir: g++ -std=c++17 -I. src/backend/fragmentation/test_fragmentation.cpp src/backend/fragmentation/Fragmenter.cpp src/backend/fragmentation/Reassembler.cpp -o fragtest


int main() {

    // test 1
    std::string message;

    message = "This is just a test. Scuba chat ++";
    test_reassembly(fragmentMessage(message));
    test_framework::test_assert(message, message);

    // test 2
    message = "We did have everything, didn't we?";
    test_reassembly(fragmentMessage(message));
    test_framework::test_assert(message, message);

    return 0;
}
