//
// Created by Andrei Ursachi on 08.04.2025.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <cstdint>
#include <string>

namespace Message_Queue {

class MessageQueue {
public:
    MessageQueue();
private:
    typedef struct {
        std::string message;
        uint8_t message_address;
    } Message;
};

} // Message_Queue

#endif //MESSAGEQUEUE_H
