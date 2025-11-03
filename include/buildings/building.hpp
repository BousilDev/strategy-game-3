#pragma once

/**
 * @file building.hpp
 * @brief Declares the Building class which is owned by a player and located on the game 
 * map. Gives the owner resources at the start of a turn.
 */

namespace buildings {

enum BuildingType {
  kCapital,
  kFarm,
  kMine,
  kNeighbourhood,
  kPower
};
  
class Building {
public:
  Building() {}

  virtual ~Building() = default;
  
  // virtual std::unique_ptr<Building> Clone() = 0;

  /**
   * @brief Get the type of the building.
   * 
   * @return The type of the building.
   */
  virtual BuildingType GetType() const {
    return kCapital;
  }
};

class CapitalBuilding : public Building {
public:
  CapitalBuilding() : Building() {}

  // std::unique_ptr<Building> Clone() override;

  BuildingType GetType() const override { return kCapital; } 
};
  
} // namespace buildings
