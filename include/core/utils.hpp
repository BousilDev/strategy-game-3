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
    
    /**
     * @brief Prints a debug message with the given arguments to the standard output stream.
     * 
     * This function is a variadic template, meaning it can accept any number of arguments of any type.
     * The function prints the given arguments to the standard output stream, prefixed with the debug prefix
     * defined in constants.hpp. The function then prints a newline character to the standard output stream.
     */
    template <typename... Types>
    // Uses universal references Types&&... to accept any number of arguments of any type
    void PrintTestMsg(Types&&... args) {
        std::cout << constants::debug_prefix;
        (std::cout << ... << args);
        std::cout << std::endl;
    }
    
    /**
     * @brief Reads a line from the input stream and returns it as a string.
     * 
     * This function is used to read lines from files and other input streams.
     * It uses std::getline to read a line from the input stream.
     * The line is then returned as a std::string.
     */
    inline std::string GetStringFromLine(std::istream& in) {
        std::string line;
        std::getline(in, line);
        return line;
    }

    /**
     * @brief Reads an integer from the input stream and returns it as an unsigned int.
     * 
     * @param in The input stream to read from.
     * @return The integer read from the input stream as an unsigned int.
     */
    inline unsigned int GetIntFromLine(std::istream& in) {
        return std::stoul(GetStringFromLine(in));
    }
    
    /**
    * @brief Reads a line from the input stream and converts it into a vector of integers.
    * 
    * The line should contain integers separated by commas, e.g., "1,2,11,4".
    * 
    * @param in The input stream to read from.
    * @return std::vector<int> A vector containing the integers from the line.
    */
    inline std::vector<int> GetIntVectorFromLine(std::istream& in) {
        std::string line;
        std::getline(in, line);  // Read the line from the stream
        std::vector<int> result;
        std::stringstream ss(line);
        std::string token;
        
        while (std::getline(ss, token, ',')) { // Split line by commas
            if (!token.empty()) {
                result.push_back(std::stoi(token)); // Convert to int and add to vector
            }
        }

        return result;
    }
    
    /**
     * @brief Throws a runtime error with a message including file and line information.
     * 
     * Example usage:
     * @code
     * ThrowWithMessage("An error occurred", __FILE__, __LINE__);
     * @endcode
     * 
     * @param message The error message.
     * @param file The file where the error occurred.
     * @param line The line number where the error occurred.
     */
    inline void ThrowWithMessage(const std::string& message, const std::string& file, int line) {
        throw std::runtime_error(message + " at " + file + ":" + std::to_string(line));
    }
    
    /**
     * @brief Reads a line from the input stream, interprets it as a std::time_t and returns a string
     * representing the time in a human-readable format.
     * 
     * @param file The input stream to read from.
     * @return A string representing the time in a human-readable format.
     */
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