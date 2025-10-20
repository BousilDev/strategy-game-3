#pragma once

#include "core/game.hpp"
#include <iostream>

namespace core {
  // Function to print test messages with debug prefix specified in constants.hpp
  template <typename... Types>
  // Uses universal references Types&&... (not sure if const works here)
  void PrintTestMsg(Types&&... args) {
    std::cout << constants::debug_prefix;
    (std::cout << ... << args);
    std::cout << std::endl;
  }
  void TestGameInitializationAndTurns();

}