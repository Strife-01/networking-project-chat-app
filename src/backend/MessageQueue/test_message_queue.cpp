//
// Created by Andrei Ursachi on 09.04.2025.
//

#include "MessageQueue.h"
#include "../../utils/test_framework.h"

int main() {
    using namespace Message_Queue;
    msg_queue.push_message(msg_queue.create_message("first message"));
    for (auto msg : msg_queue.get_messages()) {
        std::cout << "msg.message = " << msg.message << std::endl;
        std::cout << "msg.sender_address = " << static_cast<int>(msg.sender_address) << std::endl;
        std::cout << "msg.private_message = " << msg.private_message << std::endl;
        std::cout << "msg.seen_message = " << msg.seen_message << std::endl;
        std::cout << std::endl;
    }
    msg_queue.push_message(msg_queue.create_message("second message"));
    for (auto msg : msg_queue.get_messages()) {
        std::cout << "msg.message = " << msg.message << std::endl;
        std::cout << "msg.sender_address = " << static_cast<int>(msg.sender_address) << std::endl;
        std::cout << "msg.private_message = " << msg.private_message << std::endl;
        std::cout << "msg.seen_message = " << msg.seen_message << std::endl;
        std::cout << std::endl;
    }
    msg_queue.push_message(msg_queue.create_message("third message"));
    for (auto msg : msg_queue.get_messages()) {
        std::cout << "msg.message = " << msg.message << std::endl;
        std::cout << "msg.sender_address = " << static_cast<int>(msg.sender_address) << std::endl;
        std::cout << "msg.private_message = " << msg.private_message << std::endl;
        std::cout << "msg.seen_message = " << msg.seen_message << std::endl;
        std::cout << std::endl;
    }

    msg_queue.push_message(msg_queue.create_message("targeted_first", 1), 1);
    for (auto msg : msg_queue.get_messages(1)) {
        std::cout << "msg.message = " << msg.message << std::endl;
        std::cout << "msg.sender_address = " << static_cast<int>(msg.sender_address) << std::endl;
        std::cout << "msg.private_message = " << msg.private_message << std::endl;
        std::cout << "msg.seen_message = " << msg.seen_message << std::endl;
        std::cout << std::endl;
    }

    msg_queue.push_message(msg_queue.create_message("targeted_second", 1), 1);
    for (auto msg : msg_queue.get_messages(1)) {
        std::cout << "msg.message = " << msg.message << std::endl;
        std::cout << "msg.sender_address = " << static_cast<int>(msg.sender_address) << std::endl;
        std::cout << "msg.private_message = " << msg.private_message << std::endl;
        std::cout << "msg.seen_message = " << msg.seen_message << std::endl;
        std::cout << std::endl;
    }

    for (auto msg : msg_queue.get_messages(2)) {
        std::cout << "msg.message = " << msg.message << std::endl;
        std::cout << "msg.sender_address = " << static_cast<int>(msg.sender_address) << std::endl;
        std::cout << "msg.private_message = " << msg.private_message << std::endl;
        std::cout << "msg.seen_message = " << msg.seen_message << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Is unseen " << msg_queue.has_unseen_messages(1) << std::endl;
    msg_queue.push_message(msg_queue.create_message("targeted_third", 1), 1);
    std::cout << "Is unseen " << msg_queue.has_unseen_messages(1) << std::endl;
}