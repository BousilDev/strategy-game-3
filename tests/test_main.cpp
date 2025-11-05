#include "core/game.hpp"
#include "core/tests.hpp"

#include <iostream>
#include <string>

void PrintTestFailure(const std::string msg) {
    std::cerr << "Test failed: " << msg << std::endl;
}

struct test_result {
    const std::string description;
    bool passed;
};

struct test_suite {
    const std::string description;
    test_result (*test)();
};

test_result TestGame() {
    try
    {
        core::TestGameInitializationAndTurns();
        core::TestGameSaveAndLoad();
    }
    catch(const std::exception& e)
    {
        PrintTestFailure(e.what());
        return {e.what(), false};
    }
    return {"", true};
}

int main() {

    test_suite tests[] = {
        {"Testing Game Initialization and turns along with Save and Load", TestGame},
    };
    unsigned int passed = 0;
    unsigned int total = 0;
    std::vector<test_result> results;

    for (auto test : tests) {
        test_result result = test.test();
        results.push_back(result);
        if (result.passed) {
            passed++;   
        }
        total++;
    }

    std::cout << "------------------------" << std::endl;
    std::cout << "Tests run: " << std::endl;
    unsigned int i = 0;
    for (auto test : tests) {
        std::cout << constants::debug_prefix << test.description << std::endl;
        std::cout << constants::debug_prefix << "---> " << (results[i].passed ? "PASSED" : "FAILED: ") << results[i].description << std::endl;
        i++;
    }
    std::cout << "Tests passed: " << passed << "/" << total << std::endl;
    return 0;
}