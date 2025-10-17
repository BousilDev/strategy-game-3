#include "core/player.hpp"
#include <iostream>

void core::Player::AddBuilding(std::shared_ptr<buildings::Building> building) {
    buildings_.push_back(building);
}

void core::Player::RemoveBuilding(std::shared_ptr<buildings::Building> building) {
    buildings_.remove_if([&building](const std::shared_ptr<buildings::Building>& other) { return other == building; });
}

void core::Player::AddUnit(std::shared_ptr<units::Unit> unit) {
    units_.push_back(unit);
}

void core::Player::RemoveUnit(std::shared_ptr<units::Unit> unit) {
    units_.remove_if([&unit](const std::shared_ptr<units::Unit>& other) { return other == unit; });
}

void core::Player::AddResources(const std::list<Resource>& resources) {
    for (const auto& resource : resources) {
        // static_cast converts enum ResourceType to unsigned int [0, 3]
        resources_[static_cast<unsigned int>(resource.type)].amount += resource.amount;
    }
}

void core::Player::RemoveResource(Resource resource) {
    Resource& res = resources_[static_cast<unsigned int>(resource.type)];
    if (res.amount < resource.amount) {
        // Handle error: not enough resources
        // For now, set to zero (do we want negative resources?)
        res.amount = 0;
    } else {
        res.amount -= resource.amount;
    }
}

bool core::Player::IsAlive() const {
    for (const auto& building : buildings_) {
        if (building->GetType() == buildings::BuildingType::kCapital) {
            return true;
        }
    }
    return false;
}