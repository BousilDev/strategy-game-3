#include "buildings/building.hpp"
#include "world/tile.hpp"
#include "core/player.hpp"
#include <algorithm>
#include <iostream>

namespace buildings {

// ============================================================
// Building base class
// ============================================================

Building::Building(std::shared_ptr<world::Tile> tile,
                   std::shared_ptr<core::Player> owner,
                   int max_hp,
                   BuildingType type)
    : max_hp_(max_hp),
      current_hp_(max_hp),
      building_type_(type),
      current_tile_(tile),
      owner_(owner)
{
}

int Building::takeDamage(int damage)
{
    current_hp_ = std::max(0, current_hp_ - damage);
    // Remove the building if it has no HP left
    if (current_hp_ == 0) {
        if (auto tile = current_tile_.lock()) {
            tile->destroy_current_building();
        }
        if (auto owner = owner_.lock()) {
            owner->RemoveBuilding(shared_from_this());
        }
    }
    return current_hp_;
}

// ============================================================
// Central Factory — returns proper derived class
// ============================================================

std::shared_ptr<Building> Building::Create(std::shared_ptr<world::Tile> tile,
                                           std::shared_ptr<core::Player> owner,
                                           int max_hp,
                                           BuildingType type)
{
    if (!tile || !owner) return nullptr;

    std::shared_ptr<Building> building = nullptr;

    switch (type) {
        case BuildingType::kCapital:
            building = CapitalBuilding::Create(tile, owner, max_hp);
            break;
        case BuildingType::kFarm:
            building = FarmBuilding::Create(tile, owner, max_hp);
            break;
        default:
            std::cerr << "ERROR: Unsupported BuildingType in Building::Create.\n";
            return nullptr;
    }

    return building;
}

std::shared_ptr<Building> Building::CreateEmpty(int max_hp, BuildingType type)
{
    switch (type) {
        case BuildingType::kCapital: return CapitalBuilding::CreateEmpty(max_hp);
        case BuildingType::kFarm:    return FarmBuilding::CreateEmpty(max_hp);
        default:
            std::cerr << "ERROR: Unsupported BuildingType in CreateEmpty.\n";
            return nullptr;
    }
}

// ============================================================
// Serialization
// ============================================================

std::istream& operator>>(std::istream &in, std::shared_ptr<Building>& b)
{
    b->current_hp_ = core::GetIntFromLine(in);
    return in;
}

std::ostream& operator<<(std::ostream &out, const std::shared_ptr<Building>& b)
{
    out << constants::buildingTypeNames[static_cast<int>(b->GetType())] << "\n";
    out << b->getMaxHp() << "\n";
    out << b->getCurrentHp() << "\n";

    if (auto tile = b->getTile())
        out << tile->get_tile_number();
    else
        out << -1;

    return out;
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
    auto ptr = std::shared_ptr<CapitalBuilding>(
        new CapitalBuilding(tile, owner, max_hp));

    if (!tile->place_building(ptr))
        return nullptr;

    owner->AddBuilding(ptr);
    return ptr;
}

std::shared_ptr<CapitalBuilding> CapitalBuilding::CreateEmpty(int max_hp)
{
    return std::shared_ptr<CapitalBuilding>(
        new CapitalBuilding(nullptr, nullptr, max_hp));
}

std::shared_ptr<Building> CapitalBuilding::CreateEmptyFromCopy() const
{
    return std::make_shared<CapitalBuilding>(nullptr, nullptr, max_hp_);
}

void CapitalBuilding::atTurnEnd()
{
    std::cout << "at capital turn end\n";
}

// ============================================================
// FarmBuilding
// ============================================================

FarmBuilding::FarmBuilding(std::shared_ptr<world::Tile> tile,
                           std::shared_ptr<core::Player> owner,
                           int max_hp, int food_multiplier, int gold_multiplier)
    : food_multiplier_(food_multiplier), gold_multiplier_(gold_multiplier), Building(tile, owner, max_hp, BuildingType::kFarm)
{
}

std::shared_ptr<FarmBuilding> FarmBuilding::Create(
    std::shared_ptr<world::Tile> tile,
    std::shared_ptr<core::Player> owner,
    int max_hp, int food_multiplier, int gold_multiplier)
{
    auto ptr = std::shared_ptr<FarmBuilding>(
        new FarmBuilding(tile, owner, max_hp, food_multiplier, gold_multiplier));

    if (!tile->place_building(ptr))
        return nullptr;

    owner->AddBuilding(ptr);
    return ptr;
}

std::shared_ptr<FarmBuilding> FarmBuilding::CreateEmpty(int max_hp)
{
    return std::shared_ptr<FarmBuilding>(
        new FarmBuilding(nullptr, nullptr, max_hp));
}

std::shared_ptr<Building> FarmBuilding::CreateEmptyFromCopy() const
{
    return std::make_shared<FarmBuilding>(nullptr, nullptr, max_hp_);
}

void FarmBuilding::atTurnEnd()
{   
    std::cout << "at farm turn end\n";
    auto tile  = current_tile_.lock();
    auto owner = owner_.lock();

    if (!tile || !owner)
        return;

    auto terrain_type = tile->get_terrain()->get_terrain_type();

    // Farms only work on plains and forests
    if (!(terrain_type == world::Terrain::plains ||
          terrain_type == world::Terrain::forest))
        return;
    
    // Gather resources from the tile's terrain
    auto all_resources = tile->get_terrain()->get_resources();
    std::list<core::Resource> farm_resources;
    for (auto& resource : all_resources) {
        if (resource.type == core::ResourceType::kFood) {
            farm_resources.push_back(resource * food_multiplier_);
        }
        if (resource.type == core::ResourceType::kGold) {
            farm_resources.push_back(resource * gold_multiplier_);
        }
    }   

    owner->AddResources(farm_resources);
}

} // namespace buildings
