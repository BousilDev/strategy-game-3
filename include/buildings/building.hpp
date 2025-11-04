#pragma once

#include <iostream>

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

  ~Building() = default;
  
  // virtual std::unique_ptr<Building> Clone() = 0;

  /**
   * @brief Get the type of the building.
   * 
   * @return The type of the building.
   */
  BuildingType GetType() const { return BuildingType::kCapital; }

  friend std::istream& operator>>(std::istream &in, std::shared_ptr<Building>& other) {
      std::string typeStr;
      std::getline(in, typeStr);
      return in;
  };

  friend std::ostream& operator<<(std::ostream &out, const std::shared_ptr<Building>& other) {
      out << "Building";
      return out;
  };

};

class CapitalBuilding : public Building {
public:
  CapitalBuilding() : Building() {}

  // std::unique_ptr<Building> Clone() override;

  BuildingType GetType() const { return kCapital; } 

  friend std::ostream& operator<<(std::ostream &out, const std::shared_ptr<CapitalBuilding>& other) {
      out << "CapitalBuilding";
      return out;
  };
};
  
} // namespace buildings
