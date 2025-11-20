#pragma once
#include <memory>
#include <iostream>

namespace world { class Tile; }  // forward declaration
namespace core { class Player; }

namespace buildings {

enum class BuildingType {
    kCapital,
    kFarm,
    kMine,
    kNeighbourhood,
    kPower
};

class Building : public std::enable_shared_from_this<Building> {
public:
    virtual ~Building() = default;

    static std::shared_ptr<Building> Create(std::shared_ptr<world::Tile> tile,
                                            std::shared_ptr<core::Player> owner,
                                            int max_hp,
                                            BuildingType build_type);

    static std::shared_ptr<Building> CreateEmpty(int max_hp, BuildingType build_type);
    std::shared_ptr<Building> CreateEmptyFromCopy();

    bool setPlayer(std::shared_ptr<core::Player> player){
        owner_ = player;
    };
    bool setTile(std::shared_ptr<world::Tile> tile_location){
        current_tile_ = tile_location;
    };

    BuildingType GetType() const { return building_type_; }
    int getMaxHp() const { return max_hp_; }
    int getCurrentHp() const { return current_hp_; }
    int takeDamage(int damage);
    virtual void atTurnEnd() {}

    friend std::istream& operator>>(std::istream &in, std::shared_ptr<Building>& other) {
      std::string typeStr;
      std::getline(in, typeStr);
      return in;
  };

  friend std::ostream& operator<<(std::ostream &out, const std::shared_ptr<Building>& other) {
      out << "Building";
      return out;
  };


protected:
    Building(std::shared_ptr<world::Tile> tile,
             std::shared_ptr<core::Player> owner,
             int max_hp,
             BuildingType build_type);

    int max_hp_{0};
    int current_hp_{0};
    BuildingType building_type_{BuildingType::kFarm};

    std::weak_ptr<world::Tile> current_tile_;
    std::weak_ptr<core::Player> owner_;
};

// CapitalBuilding
class CapitalBuilding : public Building {
public:
    static std::shared_ptr<CapitalBuilding> Create(std::shared_ptr<world::Tile> tile,
                                                   std::shared_ptr<core::Player> owner,
                                                   int max_hp);
    static std::shared_ptr<CapitalBuilding> CreateEmpty(int max_hp);
    std::shared_ptr<CapitalBuilding> CreateEmptyFromCopy();

    void atTurnEnd() override;

    CapitalBuilding(std::shared_ptr<world::Tile> tile,
                    std::shared_ptr<core::Player> owner,
                    int max_hp);

 friend std::ostream& operator<<(std::ostream &out, const std::shared_ptr<CapitalBuilding>& other) {
      out << "CapitalBuilding";
      return out;
  };

};

// FarmBuilding
class FarmBuilding : public Building {
public:
    static std::shared_ptr<FarmBuilding> Create(std::shared_ptr<world::Tile> tile,
                                                std::shared_ptr<core::Player> owner,
                                                int max_hp);
    static std::shared_ptr<FarmBuilding> CreateEmpty(int max_hp);
    std::shared_ptr<FarmBuilding> CreateEmptyFromCopy();

    void atTurnEnd() override;

    FarmBuilding(std::shared_ptr<world::Tile> tile,
                 std::shared_ptr<core::Player> owner,
                 int max_hp);

  friend std::ostream& operator<<(std::ostream &out, const std::shared_ptr<FarmBuilding>& other) {
      out << "FarmBuilding";
      return out;
  };

};

} // namespace buildings
