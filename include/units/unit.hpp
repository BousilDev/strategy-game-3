#pragma once
#include <memory>

namespace world { class Tile; }  // forward declaration
namespace core { class Player; } // forward declaration

namespace units {

enum class UnitType {
    kSoldier,
};

class Unit : public std::enable_shared_from_this<Unit> {
public:
    virtual ~Unit() = default;

    static std::shared_ptr<Unit> Create(std::shared_ptr<world::Tile> tile,
                                        std::shared_ptr<core::Player> owner,
                                        int max_hp,
                                        UnitType unit_type);

    static std::shared_ptr<Unit> CreateEmpty(int max_hp, UnitType unit_type);
    std::shared_ptr<Unit> CreateEmptyFromCopy();

    UnitType GetType() const { return unit_type_; }
    int getMaxHp() const { return max_hp_; }
    int getCurrentHp() const { return current_hp_; }

    int takeDamage(int damage);
    bool moveToTile(std::shared_ptr<world::Tile> tile);
    void dealDamageToTileContents(std::shared_ptr<world::Tile> tile, int damage);

protected:
    Unit(std::shared_ptr<world::Tile> tile,
         std::shared_ptr<core::Player> owner,
         int max_hp,
         UnitType unit_type);

    int max_hp_{0};
    int current_hp_{0};
    bool has_attacked_{false};
    int turn_movement_{0};
    UnitType unit_type_{UnitType::kSoldier};

    std::weak_ptr<world::Tile> current_tile_;
    std::weak_ptr<core::Player> owner_;
};

class Soldier : public Unit {
public:
    static std::shared_ptr<Soldier> Create(std::shared_ptr<world::Tile> tile,
                                           std::shared_ptr<core::Player> owner,
                                           int max_hp);
    static std::shared_ptr<Soldier> CreateEmpty(int max_hp);
    std::shared_ptr<Soldier> CreateEmptyFromCopy();

    Soldier(std::shared_ptr<world::Tile> tile,
            std::shared_ptr<core::Player> owner,
            int max_hp);
};

} // namespace units
