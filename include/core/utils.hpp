# pragma once

/**
 * @file utils.hpp
 * @brief Declares utility functions
 */

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace core {
    // Function to print test messages with debug prefix specified in constants.hpp
    template <typename... Types>
    // Uses universal references Types&&... (not sure if const works here)
    void PrintTestMsg(Types&&... args) {
        std::cout << constants::debug_prefix;
        (std::cout << ... << args);
        std::cout << std::endl;
    }
    inline std::string GetStringFromLine(std::istream& in) {
        std::string line;
        std::getline(in, line);
        return line;
    }
    inline unsigned int GetIntFromLine(std::istream& in) {
        return std::stoul(GetStringFromLine(in));
    }
    inline void ThrowWithMessage(const std::string& message, const std::string& file, int line) {
        throw std::runtime_error(message + " at " + file + ":" + std::to_string(line));
    }
    inline std::string DecodeTimeFromFile(std::istream& file) {
        std::time_t timestamp = static_cast<std::time_t>(GetIntFromLine(file));
        //return asctime(std::localtime(&timestamp));
        std::tm tm{};
        localtime_r(&timestamp, &tm);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
}