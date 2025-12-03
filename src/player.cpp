#include "core/player.hpp"
#include "cards/card.hpp"
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
        ThrowWithMessage("Not enough resources", __FILE__, __LINE__);
    } else {
        res.amount -= resource.amount;
    }
}

bool core::Player::IsAlive() const {
    std::shared_ptr<buildings::Building> capital = GetCapitalBuilding();
    if (capital != nullptr && capital->getCurrentHp() > 0) {
        return true;
    }
    return false;
}

std::shared_ptr<buildings::Building> core::Player::GetCapitalBuilding() const {
    for (const auto& building : buildings_) {
        if (building->GetType() == buildings::BuildingType::kCapital) {
            return building;
        }
    }
    return nullptr;
}

namespace core {
std::ostream& operator<<(std::ostream &out, const core::Player& other) {
    out << other.GetName() << "\n";
    //out << other.deck_ << "\n";
    //out << other.hand_ << "\n";
    
    out << other.GetBuildings().size() << "\n";
    for (const auto& building : other.GetBuildings()) {
        out << building << "\n";
    }

    out << other.GetUnits().size() << "\n";
    for (const auto& unit : other.GetUnits()) {
        out << unit << "\n";
    }

    out << other.GetResources().size() << "\n";
    for (const auto& resource : other.GetResources()) {
        out << resource.amount << "\n";
    }
    return out;
}

std::istream& operator>>(std::istream &in, core::Player& other) {
    
    // get deck here
    //std::shared_ptr<cards::Deck> deck = std::make_shared<cards::Deck>();
    //in >> *deck;
    //other.SetDeck(deck);
    // TEMPORARY deck for loading TODO: Implement proper loading of deck
    std::vector<std::shared_ptr<cards::Card>> empty_cards = {std::make_shared<cards::BuildingCard>("Test card", "This is a test card", buildings::FarmBuilding::CreateEmpty(10)), std::make_shared<cards::BuildingCard>("Test card", "This is a test card", buildings::FarmBuilding::CreateEmpty(10)), std::make_shared<cards::BuildingCard>("Test card", "This is a test card", buildings::FarmBuilding::CreateEmpty(10))};
    cards::Deck test_deck = cards::Deck(empty_cards, 3U);
    other.SetDeck(test_deck.Clone());
    other.DrawHand();

    size_t resSize = GetIntFromLine(in);
    std::list<core::Resource> resources;
    // VALIDATE
    while (resSize--) {
        core::ResourceType type = static_cast<core::ResourceType>(resources.size());
        unsigned int amount = GetIntFromLine(in);
        resources.push_back(core::Resource(type, amount));
    }
    other.AddResources(resources);
    
    return in;
}
} // namespace core