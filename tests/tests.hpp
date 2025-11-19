#pragma once

#include "core/game.hpp"
#include "core/utils.hpp"
#include <iostream>

namespace core {
  inline void AssertWithMessageFull(bool condition, const std::string& message, const std::string& file, int line) {
    if (!condition) {
      ThrowWithMessage(message, file, line);
    }
  }
  void TestGameInitializationAndTurns();
  void DebugGameState(const Game& game);
  void TestGameSaveAndLoad();
}