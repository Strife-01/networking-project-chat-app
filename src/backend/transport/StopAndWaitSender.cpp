#include "StopAndWaitSender.h"
#include <chrono>
#include <cstdint>
#include <thread>
#include <iostream>

void send_packet(const packet_header& header, const std::vector<char>& payload) {
    // add send from the framework
    std::cout << "[SEND] Fragment " << header.fields.fragment_id << " of msg " << (int) header.fields.msg_id << ", payload: " << std::string(payload.begin(), payload.end()) << "\n";
}

StopAndWaitSender::StopAndWaitSender() : ackReceived( false ), current_msg_id(0), current_fragment_id(0) {}

void StopAndWaitSender::sendFragments(const std::vector<std::pair<packet_header, std::vector<char>>>& fragments) {
    for (const auto& [header, payload] : fragments) {
        current_msg_id = header.fields.msg_id;
        current_fragment_id = header.fields.fragment_id;

        ackReceived = false;
        sendWithRetry(header, payload);
    }
}

void StopAndWaitSender::sendWithRetry(const packet_header& header, const std::vector<char>& payload) {
    const int MAX_RETRIES = 5;
    int attempts = 0;

    while (!ackReceived && attempts < MAX_RETRIES) {
        send_packet(header, payload);

        std::unique_lock<std::mutex> lock(ackMutex);
        bool received = (ackCond.wait_for(lock, std::chrono::milliseconds(300), [this]{ return ackReceived.load(); }));

        if (received) {
            std::cout << "[ACK] received for fragment " << header.fields.fragment_id << "\n";
            break;
        }
        else {
            attempts ++;
            std::cout << "[TIMEOUT] Retrying fragment " << header.fields.fragment_id << " (attempts)";
            if (attempts > MAX_RETRIES) {
                std::cout << "[FAILURE] Giving up on fragment: " << header.fields.fragment_id << "\n";
                break;
            }
        }
    }



    if (!ackReceived) {
        std::cout << "[FAILURE] Giving up on fragment " << header.fields.fragment_id << "\n";
    } else {
        std::cout << "[ACK] received for fragment" << header.fields.fragment_id << "\n";
    }
}

void StopAndWaitSender::handleAck(uint8_t msg_id, uint8_t fragment_id) {
    std::lock_guard<std::mutex> lock(ackMutex);
    if (msg_id == current_msg_id && fragment_id == current_fragment_id) {
        std::lock_guard<std::mutex> lock(ackMutex);
        ackReceived = true;
        ackCond.notify_all();
    }
}

