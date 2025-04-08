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
    uint8_t get_exponential_backoff() const;

private:
    uint16_t time_to_wait;
    uint8_t exponential_backoff;
};

    inline MediumAccessControl mac_object = MediumAccessControl();

} // Medium_Access_Control

#endif //MEDIUMACCESSCONTROL_H
