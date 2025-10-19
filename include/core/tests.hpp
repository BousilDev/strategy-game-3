#pragma once

#include "core/game.hpp"
#include <sstream>

namespace core {

  template <typename T>
  void PrintTestMsg(const T& msg);
  void PrintTestMsg(const std::ostringstream& msg);
  void TestGameInitializationAndTurns();

}