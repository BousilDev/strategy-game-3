#include "core/player.hpp"
#include <iostream>
#include <string>

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

namespace core {
std::ostream& operator<<(std::ostream &out, const core::Player& other) {
    out << other.GetName() << "\n";
    //out << other.GetDeck() << "\n";
    /*
    out << other.GetBuildings().size() << "\n";
    for (const auto& building : other.GetBuildings()) {
        out << building << "\n";
    }
    out << other.GetUnits().size() << "\n";
    for (const auto& unit : other.GetUnits()) {
        out << unit << "\n";
    }
        */
    out << other.GetResources().size() << "\n";
    for (const auto& resource : other.GetResources()) {
        out << static_cast<int>(resource.type) << "\n";
        out << resource.amount << "\n";
    }
    return out;
}

std::istream& operator>>(std::istream &in, core::Player& other) {
    
    // get deck here
    /*
    std::string buildSizeStr;
    std::getline(in, buildSizeStr);
    size_t buildSize = std::stoul(buildSizeStr);
    while (buildSize--) {
        std::shared_ptr<buildings::Building> building;
        in >> building;
        other.AddBuilding(building);
    }
    std::string unitSizeStr;
    std::getline(in, unitSizeStr);
    size_t unitSize = std::stoul(unitSizeStr);
    while (unitSize--) {
        std::shared_ptr<units::Unit> unit;
        in >> unit;
        other.AddUnit(unit);
    }
        */
    std::string resSizeStr;
    std::getline(in, resSizeStr);
    size_t resSize = std::stoul(resSizeStr);
    std::list<core::Resource> resources;
    while (resSize--) {
        std::string typeStr;
        std::getline(in, typeStr);
        std::string amountStr;
        std::getline(in, amountStr);
        core::ResourceType type = static_cast<core::ResourceType>(std::stoul(typeStr));
        unsigned int amount = std::stoul(amountStr);
        resources.push_back(core::Resource(type, amount));
    }
    other.AddResources(resources);
    
    return in;
}
} // namespace core