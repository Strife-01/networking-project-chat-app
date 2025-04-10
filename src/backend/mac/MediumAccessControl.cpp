//
// Created by Strife on 08.04.2025.
//

#include "MediumAccessControl.h"

namespace Medium_Access_Control {
    MediumAccessControl::MediumAccessControl()
    : time_to_wait ( 0 ), exponential_backoff ( 0 )
    {
    }

    void MediumAccessControl::recalculate_wait_time() {
        std::lock_guard<std::mutex> lock ( mutex );
        if (this->exponential_backoff <= 10) {
            this->time_to_wait = 1 << (this->exponential_backoff);
            ++this->exponential_backoff;
        }else{
            this->exponential_backoff = 0;
        }
        this->time_to_wait = Random::get<uint16_t>(0, this->time_to_wait);
    }

    void MediumAccessControl::reset_wait_time() {
        std::lock_guard<std::mutex> lock ( mutex );
        this->exponential_backoff = 0;
        this->time_to_wait = 0;
    }

    uint16_t MediumAccessControl::get_wait_time() {
        std::lock_guard<std::mutex> lock ( mutex );
        return this->time_to_wait;
    }

    void MediumAccessControl::decrement_wait_time() {
        std::lock_guard<std::mutex> lock ( mutex );
        if (this->time_to_wait > 0) {
          --this->time_to_wait;
        }
    }

    uint8_t MediumAccessControl::get_exponential_backoff() {
        std::lock_guard<std::mutex> lock ( mutex );
        return this->exponential_backoff;
    }

} // Medium_Access_Control