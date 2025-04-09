//
// Created by Andrei Ursachi on 08.04.2025.
//

#include "MediumAccessControl.h"
#include "../../utils/test_framework.h"

int main() {
    using namespace Medium_Access_Control;
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(0) , "mac_object->exponential_backoff == 0", "Medium Access Control initial state");
    test_framework::test_assert(mac_object.get_wait_time(), static_cast<uint16_t>(0) , "mac_object->time_to_wait == 0");

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(1) , "mac_object.exponential_backoff == 1", "Medium Access Control exponential backoff");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(2) , "mac_object.exponential_backoff == 2");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(3) , "mac_object.exponential_backoff == 3");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(4) , "mac_object.exponential_backoff == 4");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(5) , "mac_object.exponential_backoff == 5");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(6) , "mac_object.exponential_backoff == 6");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(7) , "mac_object.exponential_backoff == 7");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(8) , "mac_object.exponential_backoff == 8");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(9) , "mac_object.exponential_backoff == 9");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(10) , "mac_object.exponential_backoff == 10");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    mac_object.recalculate_wait_time();
    test_framework::test_assert(mac_object.get_exponential_backoff(), static_cast<uint8_t>(10) , "mac_object.exponential_backoff == 10 (no overflow)");
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;

    uint16_t current_wait_time = mac_object.get_wait_time();
    mac_object.decrement_wait_time();
    if (current_wait_time == 0) {
        test_framework::test_assert(mac_object.get_wait_time(), static_cast<uint16_t>(0) , "", "Testing time to wait decrement", "The wait time should not decrement below 0");
    } else {
        test_framework::test_assert(mac_object.get_wait_time(), static_cast<uint16_t>(current_wait_time - static_cast<uint16_t>(1)), "", "Testing time to wait decrement");
    }
    std::cout << "[DEBUG] mac_object.time_to_wait = " << mac_object.get_wait_time() << "\n" << std::endl;


    mac_object.reset_wait_time();
    test_framework::test_assert(mac_object.get_wait_time(), static_cast<uint16_t>(0) , "", "Testing time to wait decrement", "The wait time should be 0 after reset");
}