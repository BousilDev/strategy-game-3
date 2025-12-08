#pragma once

#include <vector>

#include "cards/card.hpp"
#include "cards/deck.hpp"

/**
 * @file deck_and_card_options.hpp
 * @brief Defines individual deck and card options used in the game.
 */

namespace card_constants {

// ---------------------------------------------
// Helper for readability
// ---------------------------------------------
using core::Resource;
using core::ResourceType;

// ---------------------------------------------
// Building Cards
// ---------------------------------------------

// A cheap farm with weak output but low HP
const std::shared_ptr<cards::Card> kSmallFarmCard =
    std::make_shared<cards::BuildingCard>(
        "Small Farm",
        "Produces a small amount of food each turn.",
        Resource(ResourceType::kGold, 5),
        buildings::FarmBuilding::CreateEmpty(
            /*hp*/ 60,
            /*food_multiplier*/ 1,
            /*gold_multiplier*/ 0
        )
    );

// A stronger, gold-boosting farm
const std::shared_ptr<cards::Card> kEstateFarmCard =
    std::make_shared<cards::BuildingCard>(
        "Estate Farm",
        "A well-kept estate that yields food and a little gold.",
        Resource(ResourceType::kWood, 20),
        buildings::FarmBuilding::CreateEmpty(
            /*hp*/ 120,
            /*food_multiplier*/ 2,
            /*gold_multiplier*/ 1
        )
    );

// Balanced lumbermill
const std::shared_ptr<cards::Card> kForestryCampCard =
    std::make_shared<cards::BuildingCard>(
        "Forestry Camp",
        "Produces steady wood income.",
        Resource(ResourceType::kGold, 5),
        buildings::LumberMillBuilding::CreateEmpty(
            /*hp*/ 100,
            /*wood_multiplier*/ 1,
            /*gold_multiplier*/ 0
        )
    );

// High-output lumbermill but fragile
const std::shared_ptr<cards::Card> kLoggingStationCard =
    std::make_shared<cards::BuildingCard>(
        "Logging Station",
        "Generates large amounts of wood but is lightly built.",
        Resource(ResourceType::kMetal, 15),
        buildings::LumberMillBuilding::CreateEmpty(
            /*hp*/ 70,
            /*wood_multiplier*/ 2,
            /*gold_multiplier*/ 1
        )
    );

// A durable but low-yield mine
const std::shared_ptr<cards::Card> kStoneMineCard =
    std::make_shared<cards::BuildingCard>(
        "Stone Mine",
        "A sturdy mine extracting metal at a slow but reliable pace.",
        Resource(ResourceType::kWood, 15),
        buildings::MineBuilding::CreateEmpty(
            /*hp*/ 160,
            /*metal_multiplier*/ 1,
            /*gold_multiplier*/ 1
        )
    );

// High-yield gold-focused mine
const std::shared_ptr<cards::Card> kProspectorsMineCard =
    std::make_shared<cards::BuildingCard>(
        "Prospector's Mine",
        "A risky investment with high metal and gold output.",
        Resource(ResourceType::kWood, 50),
        buildings::MineBuilding::CreateEmpty(
            /*hp*/ 90,
            /*metal_multiplier*/ 2,
            /*gold_multiplier*/ 2
        )
    );


// ---------------------------------------------
// Unit Cards
// ---------------------------------------------

// A basic infantry soldier
const std::shared_ptr<cards::Card> kInfantryCard =
    std::make_shared<cards::UnitCard>(
        "Infantry",
        "A standard combat unit with balanced stats.",
        Resource(ResourceType::kFood, 20),
        units::Soldier::CreateEmpty(
            /*hp*/ 15,
            /*damage*/ 5
        )
    );

// A high-damage but fragile scout
const std::shared_ptr<cards::Card> kScoutCard =
    std::make_shared<cards::UnitCard>(
        "Scout",
        "Cheap, lightly armored skirmisher dealing little damage.",
        Resource(ResourceType::kFood, 5),
        units::Soldier::CreateEmpty(
            /*hp*/ 10,
            /*damage*/ 3
        )
    );

// A tankier defensive unit
const std::shared_ptr<cards::Card> kGuardianCard =
    std::make_shared<cards::UnitCard>(
        "Guardian",
        "Heavily armored defender meant to hold the line.",
        Resource(ResourceType::kGold, 30),
        units::Soldier::CreateEmpty(
            /*hp*/ 25,
            /*damage*/ 5
        )
    );

// A high-damage brute unit
const std::shared_ptr<cards::Card> kBruteCard =
    std::make_shared<cards::UnitCard>(
        "Brute",
        "High damage unit with moderate health.",
        Resource(ResourceType::kMetal, 25),
        units::Soldier::CreateEmpty(
            /*hp*/ 20,
            /*damage*/ 10
        )
    );

// ---------------------------------------------
// Vector of all available cards
// ---------------------------------------------

const std::vector<std::shared_ptr<cards::Card>> kAllAvailableCards = {
    kSmallFarmCard,
    kEstateFarmCard,
    kForestryCampCard,
    kLoggingStationCard,
    kStoneMineCard,
    kProspectorsMineCard,
    kInfantryCard,
    kScoutCard,
    kGuardianCard,
    kBruteCard
};

// ---------------------------------------------
// Decks
// ---------------------------------------------

const cards::Deck kStarterDeck(
    {
        kSmallFarmCard->Clone(),
        kSmallFarmCard->Clone(),
        kEstateFarmCard->Clone(),
        kForestryCampCard->Clone(),
        kForestryCampCard->Clone(),
        kForestryCampCard->Clone(),
        kLoggingStationCard->Clone(),
        kStoneMineCard->Clone(),
        kStoneMineCard->Clone(),
        kScoutCard->Clone(),
        kScoutCard->Clone(),
        kInfantryCard->Clone(),
        kInfantryCard->Clone(),
        kInfantryCard->Clone(),
        kInfantryCard->Clone(),
        kGuardianCard->Clone(),
        kBruteCard->Clone()
    },
    6U // hand size
);

const cards::Deck kBalancedDeck(
    {
        kSmallFarmCard->Clone(),
        kEstateFarmCard->Clone(),
        kEstateFarmCard->Clone(),
        kForestryCampCard->Clone(),
        kForestryCampCard->Clone(),
        kLoggingStationCard->Clone(),
        kStoneMineCard->Clone(),
        kStoneMineCard->Clone(),
        kProspectorsMineCard->Clone(),
        kScoutCard->Clone(),
        kScoutCard->Clone(),
        kInfantryCard->Clone(),
        kInfantryCard->Clone(),
        kGuardianCard->Clone(),
        kBruteCard->Clone()
    },
    5U // hand size
);

const cards::Deck kAggroDeck(
    {
        kEstateFarmCard->Clone(),
        kForestryCampCard->Clone(),
        kLoggingStationCard->Clone(),
        kStoneMineCard->Clone(),
        kProspectorsMineCard->Clone(),
        kScoutCard->Clone(),
        kInfantryCard->Clone(),
        kInfantryCard->Clone(),
        kBruteCard->Clone(),
        kBruteCard->Clone(),
        kBruteCard->Clone()
    },
    4U // hand size
);

const cards::Deck kEmptyDeck(
    {}, 0U
);

} // namespace card_constants

