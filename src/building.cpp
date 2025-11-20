#include "world/tile.hpp"
#include "buildings/building.hpp"
#include "core/player.hpp"
#include <algorithm>
#include <iostream>

namespace buildings {

// ============================================================
// Building (base class)
// ============================================================

Building::Building(std::shared_ptr<world::Tile> tile,
                   std::shared_ptr<core::Player> owner,
                   int max_hp,
                   BuildingType build_type)
    : max_hp_(max_hp),
      current_hp_(max_hp),
      building_type_(build_type),
      current_tile_(tile),
      owner_(owner)
{
    
}

// Factory function to safely construct and register buildings
std::shared_ptr<Building> Building::Create(std::shared_ptr<world::Tile> tile,
                                           std::shared_ptr<core::Player> owner,
                                           int max_hp,
                                           BuildingType build_type)
{
    if (!tile || !owner) return nullptr;

    auto building = std::shared_ptr<Building>(
        new Building(tile, owner, max_hp, build_type));

    // Now safe to register with tile and owner
    if (tile->place_building(building)) {
        owner->AddBuilding(building);
    }

    return building;
}
std::shared_ptr<Building> Building::CreateEmpty(int max_hp,BuildingType build_type)
{
    auto building = std::shared_ptr<Building>(
        new Building(nullptr, nullptr, max_hp, build_type));

    return building;
}
std::shared_ptr<Building> Building::CreateEmptyFromCopy(){
    auto building = std::shared_ptr<Building>(
        new Building(nullptr, nullptr, max_hp_, building_type_));
    return building;
}

int Building::takeDamage(int damage)
{
    current_hp_ = std::max(0, current_hp_ - damage);
    return current_hp_;
}

// ============================================================
// CapitalBuilding
// ============================================================

CapitalBuilding::CapitalBuilding(std::shared_ptr<world::Tile> tile,
                                 std::shared_ptr<core::Player> owner,
                                 int max_hp)
    : Building(tile, owner, max_hp, BuildingType::kCapital)
{
}

std::shared_ptr<CapitalBuilding> CapitalBuilding::Create(
    std::shared_ptr<world::Tile> tile,
    std::shared_ptr<core::Player> owner,
    int max_hp)
{
    if (!tile || !owner) return nullptr;

    auto capital = std::shared_ptr<CapitalBuilding>(
        new CapitalBuilding(tile, owner, max_hp));

    if (tile->place_building(capital)) {
        owner->AddBuilding(capital);
    }

    return capital;
}
std::shared_ptr<CapitalBuilding> CapitalBuilding::CreateEmpty(int max_hp)
{
    auto capital = std::shared_ptr<CapitalBuilding>(
        new CapitalBuilding(nullptr, nullptr, max_hp));

    return capital;
}
std::shared_ptr<CapitalBuilding> CapitalBuilding::CreateEmptyFromCopy(){
    auto building = std::shared_ptr<CapitalBuilding>(
        new CapitalBuilding(nullptr, nullptr, max_hp_));
    return building;
}

void CapitalBuilding::atTurnEnd()
{
    // Capital has no per-turn effect for now
}

// ============================================================
// FarmBuilding
// ============================================================

FarmBuilding::FarmBuilding(std::shared_ptr<world::Tile> tile,
                           std::shared_ptr<core::Player> owner,
                           int max_hp)
    : Building(tile, owner, max_hp, BuildingType::kFarm)
{
}

std::shared_ptr<FarmBuilding> FarmBuilding::Create(
    std::shared_ptr<world::Tile> tile,
    std::shared_ptr<core::Player> owner,
    int max_hp)
{
    if (!tile || !owner) return nullptr;

    auto farm = std::shared_ptr<FarmBuilding>(
        new FarmBuilding(tile, owner, max_hp));

    if (tile->place_building(farm)) {
        owner->AddBuilding(farm);
    }

    return farm;
}
std::shared_ptr<FarmBuilding> FarmBuilding::CreateEmpty(int max_hp)
{
    auto farm = std::shared_ptr<FarmBuilding>(
        new FarmBuilding(nullptr, nullptr, max_hp));

    return farm;
}
std::shared_ptr<FarmBuilding> FarmBuilding::CreateEmptyFromCopy(){
    auto building = std::shared_ptr<FarmBuilding>(
        new FarmBuilding(nullptr, nullptr, max_hp_));
    return building;
}

void FarmBuilding::atTurnEnd()
{
    // Convert weak_ptrs back to shared_ptrs before using
    auto tile = current_tile_.lock();
    auto owner = owner_.lock();

    if (!tile || !owner) return; // either was destroyed

    auto resources = tile->get_terrain()->get_resources();
    owner->AddResources(resources);
}

} // namespace buildings
