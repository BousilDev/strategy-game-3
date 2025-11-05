# pragma once

/**
 * @file utils.hpp
 * @brief Declares utility functions
 */

#include <string>
#include <iostream>

namespace core {
    inline std::string GetStringFromLine(std::istream& in) {
        std::string line;
        std::getline(in, line);
        return line;
    }
    inline unsigned int GetIntFromLine(std::istream& in) {
        return std::stoul(GetStringFromLine(in));
    }
}