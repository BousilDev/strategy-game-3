#include "core/game.hpp"
#include "tests.hpp"

#include <iostream>
#include <string>

/**
 * @brief Prints a test failure message to the standard error stream.
 * 
 * Prints the message given as an argument to the standard error stream, prefixed with
 * "Test failed: ".
 * @param msg The message to print.
 */
void PrintTestFailure(const std::string msg) {
    std::cerr << "Test failed: " << msg << std::endl;
}

struct test_result {
    const std::string description;
    bool passed;
};

struct test_suite {
    const std::string description;
    void (*test)();
};

/**
 * @brief Executes a test function and returns a test_result object containing the test description and result.
 * 
 * The function takes a test function as an argument and executes it. If the test function throws an exception,
 * the function prints a test failure message to the standard error stream and returns a test_result object with the
 * exception message as the description and false as the result. If the test function executes successfully, the function
 * returns a test_result object with an empty description and true as the result.
 * 
 * @param test The test function to execute
 * @return A test_result object containing the test description and result
 */
template <typename Func>
test_result ExecuteTest(Func test) {
    try {
        test();
    } catch (const std::exception& e) {
        PrintTestFailure(e.what());
        return {e.what(), false};
    }
    return {"", true};
}

int main() {

    // Define the tests to run
    test_suite tests[] = {
        {"Testing Game Initialization and turns", tests::TestGameInitializationAndTurns},
        {"Testing Game Save and Load", tests::TestGameSaveAndLoad},
        {"Testing Player class", tests::TestPlayer},
        {"Testing Map class", tests::TestMap},
        {"Testing Buildings", tests::TestBuildings},
        {"Testing Units", tests::TestUnits},
        {"Testing Cards", tests::TestCards},
    };
    unsigned int passed = 0;
    unsigned int total = 0;
    std::vector<test_result> results;

    // Run each test and collect results
    for (auto test : tests) {
        test_result result = ExecuteTest(test.test);
        results.push_back(result);
        if (result.passed) {
            passed++;   
        }
        total++;
    }

    // Print results
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