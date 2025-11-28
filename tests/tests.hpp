#pragma once

#include "core/game.hpp"
#include "core/utils.hpp"
#include <iostream>

namespace tests {
  inline void AssertWithMessageFull(bool condition, const std::string& message, const std::string& file, int line) {
    if (!condition) {
      core::ThrowWithMessage(message, file, line);
    }
  }
  void TestGameInitializationAndTurns();
  void DebugGameState(const core::Game& game);
  void TestGameSaveAndLoad();
}