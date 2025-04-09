//
// Created by Andrei Ursachi on 09.04.2025.
//

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <cassert>
#include <string_view>
#include <iostream>
//#include <type_traits>
//#include <functional>

namespace test_framework {
    template <typename T>
    inline void test_assert(T tested_value, T expected_value, std::string_view assertion_name = "", std::string_view test_name = "", std::string_view error_message = "") {
      std::cout << "\033[32m[TEST]: " << test_name << "\033[0m" << std::endl;
        if (!assertion_name.empty()) {
            std::cout << "    " << assertion_name << std::endl;
        }
        if (tested_value != expected_value) {
            std::cout << "\033[31m[TEST_FAILED]\033[0m:\n    Expected value: " << expected_value << "\n    Actual value: " << tested_value << std::endl;
            if (!error_message.empty()) {
                std::cout << "\033[31m    " << error_message << "\033[0m" << std::endl;
            }
            return;
        }
        std::cout << "\033[32m[TEST_PASSED]\033[0m\n" << std::endl;
    }
}

#endif //TEST_FRAMEWORK_H
