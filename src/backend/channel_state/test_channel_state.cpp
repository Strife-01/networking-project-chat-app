//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "ChannelState.h"
#include <cassert>
#include <iostream>

int main() {
    using namespace Channel_State;
    std::cout << "[TEST] Channel State initial state" << std::endl;
    std::cout << "[TEST] chan_state.is_line_busy == false" << std::endl;
    assert(chan_state.get_is_line_busy() == false);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    std::cout << "[TEST] chan_state.is_current_node_sending == false" << std::endl;
    assert(chan_state.get_is_current_node_sending() == false);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    chan_state.set_is_line_busy();
    std::cout << "[TEST] chan_state.is_line_busy == true" << std::endl;
    assert(chan_state.get_is_line_busy() == true);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    chan_state.reset_is_line_busy();
    std::cout << "[TEST] chan_state.is_line_busy == false" << std::endl;
    assert(chan_state.get_is_line_busy() == false);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    chan_state.set_is_current_node_sending();
    std::cout << "[TEST] chan_state.is_current_node_sending == true" << std::endl;
    assert(chan_state.get_is_current_node_sending() == true);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    chan_state.reset_is_current_node_sending();
    std::cout << "[TEST] chan_state.is_current_node_sending == false" << std::endl;
    assert(chan_state.get_is_current_node_sending() == false);
    std::cout << "[TEST_PASSED]\n" << std::endl;

}
