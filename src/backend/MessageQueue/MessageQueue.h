//
// Created by Andrei Ursachi on 08.04.2025.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

namespace Message_Queue {

/*
    This is the place where the final messges will be placed and the Front End
    for each individual node will collect them from here.
 */
class MessageQueue {
public:
    struct Message{
        std::string message;
        uint8_t sender_address;
        bool private_message;
        bool seen_message;
    };
    MessageQueue();
    void push_message(const Message& message);
    void push_message(const Message& message, const uint8_t sender_address);
    const std::vector<Message> get_messages(); // get bradcast messages
    const std::vector<Message> get_messages(const uint8_t sender); // get targeted messages
    bool has_unseen_messages(const uint8_t sender); // check if there are unseen messages from the sender
    Message create_message(const uint8_t sender, const std::string message, bool private_message = false, bool seen_message = false);

private:
    // 0 is broadcast; source_id for targeted
    std::unordered_map<uint8_t, std::vector<Message> > messageQueue;
    std::mutex mutex;
};

inline MessageQueue msg_queue = MessageQueue();

} // Message_Queue

#endif //MESSAGEQUEUE_H
