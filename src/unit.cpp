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
    // Can't move to null tile or if already attacked on this turn
    if (!tile || has_attacked_)
        return false;

    auto attackable = get_attackable_tiles();
    //this is an attack not move
    if (std::find(attackable.begin(), attackable.end(), tile->get_tile_number()) != attackable.end()) {
            // Tile already has a unit or a building
        if (tile->get_unit() != nullptr && tile->get_unit()->GetOwner() != GetOwner()) {
            // Damage enemy unit
            dealDamageToTileContents(tile, damage_);
            has_attacked_ = true;
            return false;
        } else if (tile->get_building() != nullptr && tile->get_building()->getOwner() != GetOwner()) {
            // Damage enemy building
            dealDamageToTileContents(tile, damage_);
            has_attacked_ = true;
            return false;
        }
    } else {
        unsigned int distance_traveled = get_terrain_distance_to(tile->get_tile_number());
        //distance traveled returns 0 if we cant move / dont move
        if (distance_traveled == 0) return false;

    
        if (tile->get_unit() != nullptr && tile->get_unit()->GetOwner() == GetOwner()) {
            // Can't move onto tile with friendly unit
            return false;
        }

        auto self = shared_from_this();
        if (tile->place_unit(self)) {
            if (auto current = current_tile_.lock()) {
                current->remove_current_unit();
            }

            turn_movement_ += static_cast<int>(distance_traveled);
            current_tile_ = tile;
            return true;
        }
    }

    return false;
}

void Unit::dealDamageToTileContents(std::shared_ptr<world::Tile> tile, int damage)
{
    if (!tile || has_attacked_)
        return;

    // Damage building
    auto targetBuilding = tile->get_building();
    if (targetBuilding != nullptr && targetBuilding->getOwner() != GetOwner()) {
        targetBuilding->takeDamage(damage);
    }

    // Damage unit
    auto targetUnit = tile->get_unit();
    if (targetUnit != nullptr && targetUnit->GetOwner() != GetOwner()) {
        targetUnit->takeDamage(damage);
    }
}
std::vector<unsigned int> Unit::get_movable_tiles() {
    std::vector<unsigned int> result; // finished tiles
    if (has_attacked_ || !current_tile_.lock()) return result;

    int depth = std::max(static_cast<int>(can_move_in_a_turn) - static_cast<int>(turn_movement_), 0);
    if (depth == 0) return result;

    std::queue<std::pair<std::shared_ptr<world::Tile>, unsigned int>> to_visit;
    std::unordered_set<unsigned int> visited;

    auto start_tile = current_tile_.lock();
    to_visit.push({start_tile, 0});
    visited.insert(start_tile->get_tile_number());

    while (!to_visit.empty()) {
        auto [tile, distance] = to_visit.front();
        to_visit.pop();

        // Skip tiles that cannot be entered
        if (tile->get_terrain()->get_terrain_type() == world::Terrain::terrainType::water) continue;
        auto unit = tile->get_unit();
        if (distance > 0 && unit != nullptr && unit->GetOwner()->GetName() != owner_.lock()->GetName()) {
            continue;
        }

        if (distance > 0 && tile->get_unit()) continue;
        if (distance > 0 && tile->get_building() && tile->get_building()->getOwner()->GetName() != owner_.lock()->GetName()) continue;

        // Add to result if it's a tile the unit can move to (exclude starting tile)
        if (distance > 0) result.push_back(tile->get_tile_number());

        // Stop expanding if max movement depth reached
        if (distance >= depth) continue;

        // Enqueue neighbors
        for (auto& weak_neigh : tile->get_neighbours()) {
            if (auto neigh = weak_neigh.lock()) {
                unsigned int neigh_number = neigh->get_tile_number();
                if (visited.find(neigh_number) == visited.end()) {
                    visited.insert(neigh_number); // mark as visited
                    to_visit.push({neigh, distance + 1});
                }
            }
        }
    }

    return result;
}

unsigned int Unit::get_terrain_distance_to(unsigned int target_tile_number) {
    if (has_attacked_ || !current_tile_.lock()) return 0;

    auto start_tile = current_tile_.lock();
    if (start_tile->get_tile_number() == target_tile_number) return 0;

    int depth = std::max(static_cast<int>(can_move_in_a_turn) - static_cast<int>(turn_movement_), 0);
    if (depth == 0) return 0;

    std::queue<std::pair<std::shared_ptr<world::Tile>, unsigned int>> to_visit;
    std::unordered_set<unsigned int> visited;

    to_visit.push({start_tile, 0});
    visited.insert(start_tile->get_tile_number());

    while (!to_visit.empty()) {
        auto [tile, distance] = to_visit.front();
        to_visit.pop();

        // Skip tiles that cannot be entered
        if (tile->get_terrain()->get_terrain_type() == world::Terrain::terrainType::water) continue;
        if (distance > 0 && tile->get_unit()) continue;

        if (tile->get_tile_number() == target_tile_number) {
            return distance; // found target, return steps
        }

        // Stop expanding if max movement depth reached
        if (distance >= depth) continue;

        for (auto& weak_neigh : tile->get_neighbours()) {
            if (auto neigh = weak_neigh.lock()) {
                unsigned int neigh_number = neigh->get_tile_number();
                if (visited.find(neigh_number) == visited.end()) {
                    visited.insert(neigh_number);
                    to_visit.push({neigh, distance + 1});
                }
            }
        }
    }

    return 0; // target not reachable
}

std::vector<unsigned int> Unit::get_attackable_tiles() {
    std::vector<unsigned int> result; // finished tiles
    if (has_attacked_ || !current_tile_.lock()) return result;

    int depth = attack_range_;
    if (depth == 0) return result;

    std::queue<std::pair<std::shared_ptr<world::Tile>, unsigned int>> to_visit;
    std::unordered_set<unsigned int> visited;

    auto start_tile = current_tile_.lock();
    to_visit.push({start_tile, 0});
    visited.insert(start_tile->get_tile_number());

    while (!to_visit.empty()) {
        auto [tile, distance] = to_visit.front();
        to_visit.pop();

        // Add to result if it's a tile the unit can move to (exclude starting tile)
        auto building = tile->get_building();
        auto unit = tile->get_unit();
        if (distance > 0 && building != nullptr && building->getOwner()->GetName() != owner_.lock()->GetName()) {
            result.push_back(tile->get_tile_number());
        }
        else if (distance > 0 && unit != nullptr && unit->GetOwner()->GetName() != owner_.lock()->GetName()) {
            result.push_back(tile->get_tile_number());
        }

        // Stop expanding if max movement depth reached
        if (distance >= depth) continue;

        // Enqueue neighbors
        for (auto& weak_neigh : tile->get_neighbours()) {
            if (auto neigh = weak_neigh.lock()) {
                unsigned int neigh_number = neigh->get_tile_number();
                if (visited.find(neigh_number) == visited.end()) {
                    visited.insert(neigh_number); // mark as visited
                    to_visit.push({neigh, distance + 1});
                }
            }
        }
    }

    return result;
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
