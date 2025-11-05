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
  inline void AssertWithMessageFull(bool condition, const std::string& message, const std::string& file, int line) {
    if (!condition) {
      throw std::runtime_error(message + " at " + file + ":" + std::to_string(line));
    }
  }
  void TestGameInitializationAndTurns();
  void DebugGameState(const Game& game);
  void TestGameSaveAndLoad();
}