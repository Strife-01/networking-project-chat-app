//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "MessageQueue.h"
#include <iostream>

namespace Message_Queue {
    MessageQueue::MessageQueue() {
        messageQueue = std::unordered_map<uint8_t, std::vector<Message> >();
        std::vector<Message> broadcastVector = std::vector<Message>();
        messageQueue.insert_or_assign(0, broadcastVector);
    }

    void MessageQueue::push_message(const MessageQueue::Message& message) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = messageQueue.find(0);
        if (it != messageQueue.end()) {
            it->second.push_back(message);
        } else {
            std::vector<Message> broadcasts = std::vector<Message>();
            broadcasts.push_back(message);
            messageQueue.insert_or_assign(0, broadcasts);
        }
    }

    void MessageQueue::push_message(const MessageQueue::Message& message, const uint8_t sender_address) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = messageQueue.find(sender_address);
        if (it != messageQueue.end()) {
            it->second.push_back(message);
        } else {
            std::vector<Message> broadcasts;
            broadcasts.push_back(message);
            messageQueue.insert_or_assign(sender_address, broadcasts);
        }
    }

    const std::vector<MessageQueue::Message> MessageQueue::get_messages() {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = messageQueue.find(0);
        if (it == messageQueue.end()) {
            return std::vector<Message>();
        }
        return it->second;
    }

    const std::vector<MessageQueue::Message> MessageQueue::get_messages(const uint8_t sender) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = messageQueue.find(sender);
        if (it == messageQueue.end()) {
            return std::vector<Message>();
        }
        // Turn messages to seen
        for (auto& message : it->second) {
            if (!message.seen_message) {
                message.seen_message = true;
            }
        }
        return it->second;
    }

    bool MessageQueue::has_unseen_messages(const uint8_t sender) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = messageQueue.find(sender);
        if (it != messageQueue.end() && !it->second.empty()) {
            const auto& lastMessage = it->second.back();
            return !lastMessage.seen_message;
        }
        return false;
    }

    MessageQueue::Message MessageQueue::create_message(const uint8_t sender, const std::string message, bool private_message, bool seen_message) {
        return MessageQueue::Message{message, sender, private_message, seen_message};
    }

} // Message_Queue