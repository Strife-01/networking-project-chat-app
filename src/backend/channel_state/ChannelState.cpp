//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "ChannelState.h"

namespace Channel_State {
    ChannelState::ChannelState()
    : is_line_busy ( false ), is_current_node_sending ( false ) {
    }

    void ChannelState::set_is_line_busy() {
        this->is_line_busy = true;
    }

    void ChannelState::reset_is_line_busy() {
        this->is_line_busy = false;
    }

    bool ChannelState::get_is_line_busy() const {
        return this->is_line_busy;
    }

    void ChannelState::set_is_current_node_sending() {
        this->is_current_node_sending = true;
    }
    void ChannelState::reset_is_current_node_sending() {
        this->is_current_node_sending = false;
    }
    bool ChannelState::get_is_current_node_sending() const {
        return this->is_current_node_sending;
    }
} // Channel_State