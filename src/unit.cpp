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

// Factory function to safely construct and register units
std::shared_ptr<Unit> Unit::Create(std::shared_ptr<world::Tile> tile,
                                   std::shared_ptr<core::Player> owner,
                                   int max_hp,
                                   UnitType unit_type)
{
    // Use make_shared for exception safety
    auto unit = std::shared_ptr<Unit>(new Unit(tile, owner, max_hp, unit_type));

    // Register with tile and owner if possible
    if (tile && tile->place_unit(unit)) {
        if (owner) {
            owner->AddUnit(unit);
        }
    }

    return unit;
}
std::shared_ptr<Unit> Unit::CreateEmpty(int max_hp, UnitType unit_type){
    auto unit = std::shared_ptr<Unit>(new Unit(nullptr, nullptr, max_hp, unit_type));
    return unit;
};
std::shared_ptr<Unit> Unit::CreateEmptyFromCopy(){
    auto unit = std::shared_ptr<Unit>(new Unit(nullptr, nullptr, max_hp_, unit_type_));
    return unit;
};

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
        // Safely unlock weak_ptr to access current tile
        if (auto current = current_tile_.lock()) {
            current->remove_current_unit();
        }

        // Update weak reference
        current_tile_ = tile;
        return true;
    }
    return false;
}

void Unit::dealDamageToTileContents(std::shared_ptr<world::Tile> tile, int damage)
{
    if (!tile)
        return;

    // Damage building, if any
    if (auto targetBuilding = tile->get_building()) {
        targetBuilding->takeDamage(damage);
    }

    // Damage unit, if any
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
    auto soldier = std::shared_ptr<Soldier>(new Soldier(tile, owner, max_hp));

    if (tile && tile->place_unit(soldier)) {
        if (owner) {
            owner->AddUnit(soldier);
        }
    }

    return soldier;
}
std::shared_ptr<Soldier> Soldier::CreateEmpty(int max_hp){
    auto unit = std::shared_ptr<Soldier>(new Soldier(nullptr, nullptr, max_hp));
    return unit;
};
std::shared_ptr<Soldier> Soldier::CreateEmptyFromCopy(){
    auto unit = std::shared_ptr<Soldier>(new Soldier(nullptr, nullptr, max_hp_));
    return unit;
};

} // namespace units
