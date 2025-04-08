//
// Created by Strife on 08.04.2025.
//

#include "MediumAccessControl.h"

namespace Medium_Access_Control {
    MediumAccessControl::MediumAccessControl()
    : time_to_wait ( 0 ), exponential_backoff ( 0 ), is_line_busy ( false )
    {
    }

    void MediumAccessControl::recalculate_wait_time() {
        if (this->exponential_backoff < 10) {
            this->time_to_wait = 1 << (this->exponential_backoff);
            ++this->exponential_backoff;
            this->time_to_wait = Random::get<uint16_t>(0, this->time_to_wait);
        }
    }

    void MediumAccessControl::reset_wait_time() {
        this->exponential_backoff = 0;
        this->time_to_wait = 0;
    }

    const uint16_t MediumAccessControl::get_wait_time() const {
        return this->time_to_wait;
    }

    void MediumAccessControl::decrement_wait_time() {
        --this->time_to_wait;
    }

    void MediumAccessControl::set_is_line_busy() {
        this->is_line_busy = true;
    }

    void MediumAccessControl::reset_is_line_busy() {
        this->is_line_busy = false;
    }

    const bool MediumAccessControl::get_is_line_busy() const {
        return this->is_line_busy;
    }


} // Medium_Access_Control