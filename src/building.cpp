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
                   BuildingType type,
                   int max_hp,
                   int resource_multiplier,
                   int gold_multiplier)
    : max_hp_(max_hp),
      current_hp_(max_hp),
      gold_multiplier_(gold_multiplier),
      resource_multiplier_(resource_multiplier),
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
                                           BuildingType type,
                                           int max_hp,
                                           int resource_multiplier,
                                           int gold_multiplier)
{
    if (!tile || !owner) return nullptr;

    std::shared_ptr<Building> building = nullptr;

    switch (type) {
        case BuildingType::kCapital:
            building = CapitalBuilding::Create(tile, owner, max_hp);
            break;
        case BuildingType::kFarm:
            building = FarmBuilding::Create(tile, owner, max_hp, resource_multiplier, gold_multiplier);
            break;
        case BuildingType::kLumberMill:
            building = LumberMillBuilding::Create(tile, owner, max_hp, resource_multiplier, gold_multiplier);
            break;
        case BuildingType::kMine:
            building = MineBuilding::Create(tile, owner, max_hp, resource_multiplier, gold_multiplier);
            break;
        default:
            std::cerr << "ERROR: Unsupported BuildingType in Building::Create.\n";
            return nullptr;
    }

    return building;
}

std::shared_ptr<Building> Building::CreateEmpty(BuildingType type, int max_hp,
                                                int resource_multiplier,
                                                int gold_multiplier)
{
    switch (type) {
        case BuildingType::kCapital:    return CapitalBuilding::CreateEmpty(max_hp);
        case BuildingType::kFarm:       return FarmBuilding::CreateEmpty(max_hp, resource_multiplier, gold_multiplier);
        case BuildingType::kLumberMill: return LumberMillBuilding::CreateEmpty(max_hp, resource_multiplier, gold_multiplier);
        case BuildingType::kMine:       return MineBuilding::CreateEmpty(max_hp, resource_multiplier, gold_multiplier);
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
    b->resource_multiplier_ = core::GetIntFromLine(in);
    b->gold_multiplier_ = core::GetIntFromLine(in);
    return in;
}

std::ostream& operator<<(std::ostream &out, const std::shared_ptr<Building>& b)
{
    out << constants::buildingTypeNames[static_cast<int>(b->GetType())] << "\n";
    out << b->getMaxHp() << "\n";
    out << b->getCurrentHp() << "\n";
    out << b->getResourceMultiplier() << "\n";
    out << b->getGoldMultiplier() << "\n";

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
    : Building(tile, owner, BuildingType::kCapital, max_hp)
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
    owner_.lock()->AddResources({core::Resource(core::ResourceType::kGold, 1)});
}

// ============================================================
// FarmBuilding
// ============================================================

FarmBuilding::FarmBuilding(std::shared_ptr<world::Tile> tile,
                           std::shared_ptr<core::Player> owner,
                           int max_hp, int food_multiplier, int gold_multiplier)
    : Building(tile, owner, BuildingType::kFarm, max_hp, food_multiplier, gold_multiplier)
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

std::shared_ptr<FarmBuilding> FarmBuilding::CreateEmpty(int max_hp, int food_multiplier, int gold_multiplier)
{
    return std::shared_ptr<FarmBuilding>(
        new FarmBuilding(nullptr, nullptr, max_hp, food_multiplier, gold_multiplier));
}

std::shared_ptr<Building> FarmBuilding::CreateEmptyFromCopy() const
{
    return std::make_shared<FarmBuilding>(nullptr, nullptr, max_hp_, resource_multiplier_, gold_multiplier_);
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
            farm_resources.push_back(resource * resource_multiplier_);
        }
        if (resource.type == core::ResourceType::kGold) {
            farm_resources.push_back(resource * gold_multiplier_);
        }
    }   

    owner->AddResources(farm_resources);
}

// ============================================================
// LumberMillBuilding
// ============================================================

LumberMillBuilding::LumberMillBuilding(std::shared_ptr<world::Tile> tile,
                           std::shared_ptr<core::Player> owner,
                           int max_hp, int wood_multiplier, int gold_multiplier)
    : Building(tile, owner, BuildingType::kFarm, max_hp, wood_multiplier, gold_multiplier)
{
}

std::shared_ptr<LumberMillBuilding> LumberMillBuilding::Create(
    std::shared_ptr<world::Tile> tile,
    std::shared_ptr<core::Player> owner,
    int max_hp, int wood_multiplier, int gold_multiplier)
{
    auto ptr = std::shared_ptr<LumberMillBuilding>(
        new LumberMillBuilding(tile, owner, max_hp, wood_multiplier, gold_multiplier));

    if (!tile->place_building(ptr))
        return nullptr;

    owner->AddBuilding(ptr);
    return ptr;
}

std::shared_ptr<LumberMillBuilding> LumberMillBuilding::CreateEmpty(int max_hp, int wood_multiplier, int gold_multiplier)
{
    return std::shared_ptr<LumberMillBuilding>(
        new LumberMillBuilding(nullptr, nullptr, max_hp, wood_multiplier, gold_multiplier));
}

std::shared_ptr<Building> LumberMillBuilding::CreateEmptyFromCopy() const
{
    return std::make_shared<LumberMillBuilding>(nullptr, nullptr, max_hp_, resource_multiplier_, gold_multiplier_);
}

void LumberMillBuilding::atTurnEnd()
{   
    std::cout << "at lumber mill turn end\n";
    auto tile  = current_tile_.lock();
    auto owner = owner_.lock();

    if (!tile || !owner)
        return;

    auto terrain_type = tile->get_terrain()->get_terrain_type();

    // Lumber Mills only work on forests and plains
    if (!(terrain_type == world::Terrain::forest ||
          terrain_type == world::Terrain::plains))
        return;
    
    // Gather resources from the tile's terrain
    auto all_resources = tile->get_terrain()->get_resources();
    std::list<core::Resource> lumbermill_resources;
    for (auto& resource : all_resources) {
        if (resource.type == core::ResourceType::kWood) {
            lumbermill_resources.push_back(resource * resource_multiplier_);
        }
        if (resource.type == core::ResourceType::kGold) {
            lumbermill_resources.push_back(resource * gold_multiplier_);
        }
    }   

    owner->AddResources(lumbermill_resources);
} 

// ============================================================
// MineBuilding
// ============================================================

MineBuilding::MineBuilding(std::shared_ptr<world::Tile> tile,
                           std::shared_ptr<core::Player> owner,
                           int max_hp, int metal_multiplier, int gold_multiplier)
    : Building(tile, owner, BuildingType::kFarm, max_hp, metal_multiplier, gold_multiplier)
{
}

std::shared_ptr<MineBuilding> MineBuilding::Create(
    std::shared_ptr<world::Tile> tile,
    std::shared_ptr<core::Player> owner,
    int max_hp, int metal_multiplier, int gold_multiplier)
{
    auto ptr = std::shared_ptr<MineBuilding>(
        new MineBuilding(tile, owner, max_hp, metal_multiplier, gold_multiplier));

    if (!tile->place_building(ptr))
        return nullptr;

    owner->AddBuilding(ptr);
    return ptr;
}

std::shared_ptr<MineBuilding> MineBuilding::CreateEmpty(int max_hp, int metal_multiplier, int gold_multiplier)
{
    return std::shared_ptr<MineBuilding>(
        new MineBuilding(nullptr, nullptr, max_hp, metal_multiplier, gold_multiplier));
}

std::shared_ptr<Building> MineBuilding::CreateEmptyFromCopy() const
{
    return std::make_shared<MineBuilding>(nullptr, nullptr, max_hp_, resource_multiplier_, gold_multiplier_);
}

void MineBuilding::atTurnEnd()
{   
    std::cout << "at mine turn end\n";
    auto tile  = current_tile_.lock();
    auto owner = owner_.lock();

    if (!tile || !owner)
        return;

    auto terrain_type = tile->get_terrain()->get_terrain_type();

    // Mines only work on mountains and hills
    if (terrain_type != world::Terrain::mountains)
        return;
    
    // Gather resources from the tile's terrain
    auto all_resources = tile->get_terrain()->get_resources();
    std::list<core::Resource> mine_resources;
    for (auto& resource : all_resources) {
        if (resource.type == core::ResourceType::kMetal) {
            mine_resources.push_back(resource * resource_multiplier_);
        }
        if (resource.type == core::ResourceType::kGold) {
            mine_resources.push_back(resource * gold_multiplier_);
        }
    }   

    owner->AddResources(mine_resources);
}

} // namespace buildings
