#include "units/unit.hpp"
#include "world/tile.hpp"
#include "core/player.hpp"
#include <algorithm>

namespace units {

// ============================================================
// Unit (base class)
// ============================================================

Unit::Unit(std::shared_ptr<world::Tile> tile,
           std::shared_ptr<core::Player> owner,
           int max_hp,
           UnitType unit_type)
    : max_hp_(max_hp),
      current_hp_(max_hp),
      unit_type_(unit_type),
      current_tile_(tile),
      owner_(owner)
{
}

// Polymorphic Create() - dispatch by UnitType
std::shared_ptr<Unit> Unit::Create(std::shared_ptr<world::Tile> tile,
                                   std::shared_ptr<core::Player> owner,
                                   int max_hp,
                                   UnitType type)
{
    switch (type) {

        case UnitType::kSoldier:
            return Soldier::Create(tile, owner, max_hp);
    }

    return nullptr;
}

// Polymorphic CreateEmpty() - dispatch by UnitType
std::shared_ptr<Unit> Unit::CreateEmpty(int max_hp, UnitType type)
{
    switch (type) {

        case UnitType::kSoldier:
            return Soldier::CreateEmpty(max_hp);
    }

    return nullptr;
}

// ============================================================
// Common Unit behavior
// ============================================================

int Unit::takeDamage(int damage)
{
    current_hp_ = std::max(0, current_hp_ - damage);
    return current_hp_;
}

bool Unit::moveToTile(std::shared_ptr<world::Tile> tile)
{
    if (!tile)
        return false;

    auto self = shared_from_this();

    if (tile->place_unit(self)) {
        if (auto current = current_tile_.lock()) {
            current->remove_current_unit();
        }

        current_tile_ = tile;
        return true;
    }

    return false;
}

void Unit::dealDamageToTileContents(std::shared_ptr<world::Tile> tile, int damage)
{
    if (!tile)
        return;

    // Damage building
    if (auto targetBuilding = tile->get_building()) {
        targetBuilding->takeDamage(damage);
    }

    // Damage unit
    if (auto targetUnit = tile->get_unit()) {
        targetUnit->takeDamage(damage);
    }
}

// ============================================================
// Soldier
// ============================================================

Soldier::Soldier(std::shared_ptr<world::Tile> tile,
                 std::shared_ptr<core::Player> owner,
                 int max_hp)
    : Unit(tile, owner, max_hp, UnitType::kSoldier)
{
}

std::shared_ptr<Soldier> Soldier::Create(std::shared_ptr<world::Tile> tile,
                                         std::shared_ptr<core::Player> owner,
                                         int max_hp)
{
    auto soldier = std::make_shared<Soldier>(tile, owner, max_hp);

    if (tile && tile->place_unit(soldier)) {
        if (owner) {
            owner->AddUnit(soldier);
        }
    }

    return soldier;
}

std::shared_ptr<Soldier> Soldier::CreateEmpty(int max_hp)
{
    return std::make_shared<Soldier>(nullptr, nullptr, max_hp);
}

std::shared_ptr<Unit> Soldier::CreateEmptyFromCopy() const
{
    return std::make_shared<Soldier>(nullptr, nullptr, max_hp_);
}

} // namespace units
