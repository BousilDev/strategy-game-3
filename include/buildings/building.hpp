#pragma once
#include <memory>
#include <iostream>

#include "constants/constants.hpp"
#include "core/utils.hpp"

namespace world { class Tile; }
namespace core  { class Player; }

namespace buildings {

enum class BuildingType {
    kCapital,
    kFarm,
    kMine,
    kNeighbourhood,
    kPower
};

// ============================================================
// Base Building
// ============================================================

class Building : public std::enable_shared_from_this<Building> {
public:
    virtual ~Building() = default;

    // Central factory that returns correct derived class
    static std::shared_ptr<Building> Create(std::shared_ptr<world::Tile> tile,
                                            std::shared_ptr<core::Player> owner,
                                            int max_hp,
                                            BuildingType type);

    // Empty creation (mainly for loading/saving)
    static std::shared_ptr<Building> CreateEmpty(int max_hp, BuildingType type);

    // Proper polymorphic clone
    virtual std::shared_ptr<Building> CreateEmptyFromCopy() const = 0;

    // setters
    void setPlayer(std::shared_ptr<core::Player> player) { owner_ = player; }
    void setTile(std::shared_ptr<world::Tile> tile)      { current_tile_ = tile; }

    // getters
    BuildingType GetType() const { return building_type_; }
    std::shared_ptr<core::Player> getOwner() const { return owner_.lock(); }
    std::shared_ptr<world::Tile>  getTile()  const { return current_tile_.lock(); }
    int getMaxHp()     const { return max_hp_; }
    int getCurrentHp() const { return current_hp_; }

    int takeDamage(int damage);
    virtual void atTurnEnd() {}

    // Serialization
    friend std::istream& operator>>(std::istream& in,  std::shared_ptr<Building>& other);
    friend std::ostream& operator<<(std::ostream& out, const std::shared_ptr<Building>& other);

protected:
    Building(std::shared_ptr<world::Tile> tile,
             std::shared_ptr<core::Player> owner,
             int max_hp,
             BuildingType type);

protected:
    int max_hp_{0};
    int current_hp_{0};
    BuildingType building_type_{BuildingType::kFarm};

    std::weak_ptr<world::Tile>  current_tile_;
    std::weak_ptr<core::Player> owner_;
};


// ============================================================
// Capital Building
// ============================================================

class CapitalBuilding : public Building {
public:
    static std::shared_ptr<CapitalBuilding> Create(std::shared_ptr<world::Tile> tile,
                                                   std::shared_ptr<core::Player> owner,
                                                   int max_hp);

    static std::shared_ptr<CapitalBuilding> CreateEmpty(int max_hp);

    std::shared_ptr<Building> CreateEmptyFromCopy() const override;

    void atTurnEnd() override;

    CapitalBuilding(std::shared_ptr<world::Tile> tile,
                    std::shared_ptr<core::Player> owner,
                    int max_hp);
};


// ============================================================
// Farm Building
// ============================================================

class FarmBuilding : public Building {
public:
    static std::shared_ptr<FarmBuilding> Create(std::shared_ptr<world::Tile> tile,
                                                std::shared_ptr<core::Player> owner,
                                                int max_hp);

    static std::shared_ptr<FarmBuilding> CreateEmpty(int max_hp);

    std::shared_ptr<Building> CreateEmptyFromCopy() const override;

    void atTurnEnd() override;

    FarmBuilding(std::shared_ptr<world::Tile> tile,
                 std::shared_ptr<core::Player> owner,
                 int max_hp);
};

} // namespace buildings
