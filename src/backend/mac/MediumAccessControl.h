//
// Created by Strife on 08.04.2025.
//
#ifndef MEDIUMACCESSCONTROL_H
#define MEDIUMACCESSCONTROL_H
#include <cstdint>
#include "../../utils/Random.h"

namespace Medium_Access_Control {

class MediumAccessControl {
public:
    MediumAccessControl();
    void recalculate_wait_time();
    void reset_wait_time();
    uint16_t get_wait_time() const;
    void decrement_wait_time();
    void set_is_line_busy();
    void reset_is_line_busy();
    bool get_is_line_busy() const;
    uint8_t get_exponential_backoff() const;

private:
    uint16_t time_to_wait;
    uint8_t exponential_backoff;
    bool is_line_busy;
};

    inline MediumAccessControl mac_object = MediumAccessControl();

} // Medium_Access_Control

#endif //MEDIUMACCESSCONTROL_H
