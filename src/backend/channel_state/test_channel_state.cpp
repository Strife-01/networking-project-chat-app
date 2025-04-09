//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "ChannelState.h"
#include "../../utils/test_framework.h"

int main() {
    using namespace Channel_State;
    test_framework::test_assert(chan_state.get_is_line_busy(), false, "chan_state.is_line_busy == false", "Channel State initial state");
    test_framework::test_assert(chan_state.get_is_current_node_sending(), false, "chan_state.is_current_node_sending == false");

    chan_state.set_is_line_busy();
    test_framework::test_assert(chan_state.get_is_line_busy(), true, "chan_state.is_line_busy == true");

    chan_state.reset_is_line_busy();
    test_framework::test_assert(chan_state.get_is_line_busy(), false, "chan_state.is_line_busy == false");

    chan_state.set_is_current_node_sending();
    test_framework::test_assert(chan_state.get_is_current_node_sending(), true, "chan_state.is_current_node_sending == true", "");

    chan_state.reset_is_current_node_sending();
    test_framework::test_assert(chan_state.get_is_current_node_sending(), false, "chan_state.is_current_node_sending == false");

}
