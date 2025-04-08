//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "MediumAccessControl.h"
#include <iostream>
#include <cassert>

int main() {
    using namespace Medium_Access_Control;
    std::cout << "[TEST] Medium Access Control initial state" << std::endl;
    std::cout << "[TEST] mac_object->exponential_backoff == 0" << std::endl;
    assert(mac_object.get_exponential_backoff() == 0);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    std::cout << "[TEST] mac_object->time_to_wait == 0" << std::endl;
    assert(mac_object.get_wait_time() == 0);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    std::cout << "[TEST] Medium Access Control exponential backoff" << std::endl;
    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 1" << std::endl;
    assert(mac_object.get_exponential_backoff() == 1);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 2" << std::endl;
    assert(mac_object.get_exponential_backoff() == 2);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 3" << std::endl;
    assert(mac_object.get_exponential_backoff() == 3);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 4" << std::endl;
    assert(mac_object.get_exponential_backoff() == 4);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 5" << std::endl;
    assert(mac_object.get_exponential_backoff() == 5);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 6" << std::endl;
    assert(mac_object.get_exponential_backoff() == 6);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 7" << std::endl;
    assert(mac_object.get_exponential_backoff() == 7);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 8" << std::endl;
    assert(mac_object.get_exponential_backoff() == 8);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 9" << std::endl;
    assert(mac_object.get_exponential_backoff() == 9);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 10" << std::endl;
    assert(mac_object.get_exponential_backoff() == 10);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.recalculate_wait_time();
    std::cout << "[TEST] mac_object.exponential_backoff == 10 && no more" << std::endl;
    assert(mac_object.get_exponential_backoff() == 10);
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << std::endl;
    std::cout << "[TEST_PASSED]\n" << std::endl;

    uint8_t current_wait_time = mac_object.get_wait_time();
    std::cout << "[TEST] decrement works" << std::endl;
    mac_object.decrement_wait_time();
    assert(mac_object.get_wait_time() == current_wait_time - 1);
    std::cout << "[TEST_PASSED]\n" << std::endl;

    mac_object.reset_wait_time();
    std::cout << "[TEST] reset_wait_time" << std::endl;
    assert(mac_object.get_wait_time() == 0);
    std::cout << "[TEST_PASSED]\n" << std::endl;
}