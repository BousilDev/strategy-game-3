#pragma once

/**
 * @file unit.hpp
 * @brief Declares the Unit class which is a deployable and movable unit owned by the 
 * player and operating on the game map. Units are used to attack and defend.
 */

namespace units {
  
class Unit {
public:
  virtual std::unique_ptr<Unit> Clone() = 0;
  
private:

};
  
} // namespace units
