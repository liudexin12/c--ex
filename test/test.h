#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        std::cerr << "Assertion failed: " << #expected << " == " << #actual << ", " \
                  << "expected: " << (expected) << ", actual: " << (actual) << std::endl; \
        return false; \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " << #condition << std::endl; \
        return false; \
    }


class TestRunner {
public:
    void addTest(const std::string& testName, std::function<bool()> testFunc) {
        tests_.emplace_back(testName, testFunc);
    }

    void run() {
        int passed = 0;
        for (const auto& test : tests_) {
            std::cout << "Running test: " << test.first << " ... ";
            if (test.second()) {
                std::cout << "PASSED" << std::endl;
                ++passed;
            } else {
                std::cout << "FAILED" << std::endl;
            }
        }
        std::cout << "Tests passed: " << passed << "/" << tests_.size() << std::endl;
    }

private:
    std::vector<std::pair<std::string, std::function<bool()>>> tests_;
};


#endif
