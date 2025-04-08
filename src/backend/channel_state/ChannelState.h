//
// Created by Andrei Ursachi on 08.04.2025.
//

#ifndef CHANNELSTATE_H
#define CHANNELSTATE_H

namespace Channel_State {

class ChannelState {
public:
    ChannelState();
    void set_is_line_busy();
    void reset_is_line_busy();
    bool get_is_line_busy() const;
    void set_is_current_node_sending();
    void reset_is_current_node_sending();
    bool get_is_current_node_sending() const;
private:
    bool is_line_busy;
    bool is_current_node_sending;
};

inline ChannelState chan_state = ChannelState();

} // Channel_State

#endif //CHANNELSTATE_H
