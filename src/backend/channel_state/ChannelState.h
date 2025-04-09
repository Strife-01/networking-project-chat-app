//
// Created by Andrei Ursachi on 08.04.2025.
//

#ifndef CHANNELSTATE_H
#define CHANNELSTATE_H
#include <mutex>

namespace Channel_State {

class ChannelState {
public:
    ChannelState();
    void set_is_line_busy();
    void reset_is_line_busy();
    bool get_is_line_busy();
    void set_is_current_node_sending();
    void reset_is_current_node_sending();
    bool get_is_current_node_sending();
private:
    bool is_line_busy;
    bool is_current_node_sending;
    std::mutex mutex;
};

inline ChannelState chan_state = ChannelState();

} // Channel_State

#endif //CHANNELSTATE_H
