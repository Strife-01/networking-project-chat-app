#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cassert>

#include "Fragmenter.h"
#include "Reassembler.h"

uint8_t generate_msg_id() {
    static uint8_t counter = 0;
    counter ++;
}
