#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cassert>

#include "Fragmenter.h"
#include "Reassembler.h"

uint8_t generate_msg_id() {
    static uint8_t counter = 0;
    return counter ++;
}

void test_basic_functionality() {
    std::string input = "I just hope that one day- preferably when we're both drunk, we can talk about it - J.D. Salinger";

    std::vector<char> original_msg(input.begin(), input.end());

    uint8_t src = 1;
    uint8_t dst = 2;
    uint8_t nextHop = 3;
    uint8_t msg_id = generate_msg_id();
    uint8_t type = 1;

    auto fragments = Fragmenter::fragmentMessage(original_msg, src, dst, nextHop, msg_id, type);

    // for (auto i : fragments) {
    //     std::cout << "[TEST] fragment " << i.h.fields.fragment_id << " payload " << i.
    // }

    std::cout << "[TEST] Total nmber of fragments: " << fragments.size() << std::endl;

    Reassembler reassembler;

    std::vector<char> result;
    for (const auto& [header, payload] : fragments) {
        result = reassembler.insertFragment(
            header.fields.msg_id,
            header.fields.fragment_id,
            header.fields.MF,
            payload
        );
    }

    std::string output(result.begin(), result.end());

    std:: cout << "[TEST] The reassembled message: " << output << "\n";

    assert(output == input);
    std::cout << "[PASS] The message of correctly fragmented and reassembled" << "\n";

}

// to run this from the root dir: g++ -std=c++17 -I. fragmentation/test_fragmentation.cpp fragmentation/Fragmenter.cpp fragmentation/Reassembler.cpp -o fragtest ./fragtest

int main() {
    test_basic_functionality();
    return 0;
}
