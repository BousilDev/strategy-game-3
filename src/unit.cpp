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

std::istream& operator>>(std::istream &in, std::shared_ptr<Unit>& other) {
    other->current_hp_ = core::GetIntFromLine(in);
    other->has_attacked_ = static_cast<bool>(core::GetIntFromLine(in));
    other->turn_movement_ = core::GetIntFromLine(in);

    return in;
}

std::ostream& operator<<(std::ostream &out, const std::shared_ptr<Unit>& other) {
    out << constants::unitTypeNames[static_cast<int>(other->GetType())] << "\n";
    out << other->getMaxHp() << "\n";
    out << other->getCurrentHp() << "\n";
    out << other->has_attacked_ << "\n";
    out << other->turn_movement_ << "\n";
    if (auto tile = other->current_tile_.lock()) {
        out << tile->get_tile_number();
    } else {
        out << -1;
    }

    return out;
}

// ============================================================
// Common Unit behavior
// ============================================================

int Unit::takeDamage(int damage)
{
    current_hp_ = std::max(0, current_hp_ - damage);
    if (current_hp_ == 0) {
        // Remove unit from its tile
        if (auto tile = current_tile_.lock()) {
            tile->remove_current_unit();
        }
        // Remove unit from its owner's list
        if (auto owner = owner_.lock()) {
            owner->RemoveUnit(shared_from_this());
        }
    }
    return current_hp_;
}

bool Unit::moveToTile(std::shared_ptr<world::Tile> tile)
{
    if (!tile)
        return false;

    auto self = shared_from_this();

    // Tile already has a unit or a building
    if (tile->get_unit() != nullptr && tile->get_unit()->getOwner() != getOwner()) {
        // Damage enemy unit
        dealDamageToTileContents(tile, damage_);
        has_attacked_ = true;
        if (tile->get_unit() != nullptr) {
            return false; // Enemy unit still alive, can't move
        }
    } else if (tile->get_building() != nullptr && tile->get_building()->getOwner() != getOwner()) {
        // Damage enemy building
        dealDamageToTileContents(tile, damage_);
        has_attacked_ = true;
        if (tile->get_building() != nullptr) {
            return false; // Enemy building still alive, can't move
        }
    } else if (tile->get_unit() != nullptr && tile->get_unit()->getOwner() == getOwner()) {
        // Can't move onto tile with friendly unit
        return false;
    }

    if (tile->place_unit(self)) {
        if (auto current = current_tile_.lock()) {
            current->remove_current_unit();
        }

        // TODO: Uncomment when GetDistanceTo or similar is implemented
        //turn_movement_ = tile->GetDistanceTo(current_tile_.lock());
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
    auto targetBuilding = tile->get_building();
    if (targetBuilding != nullptr && targetBuilding->getOwner() != getOwner()) {
        targetBuilding->takeDamage(damage);
    }

    // Damage unit
    auto targetUnit = tile->get_unit();
    if (targetUnit != nullptr && targetUnit->getOwner() != getOwner()) {
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
