#pragma once

#include <SFML/Graphics.hpp>

/**
 * @file constants.hpp
 * @brief Defines the Constants namespace which declares all global constants.
 */

namespace constants {
const int kNumberOfResourceTypes = 4;
const int kNumberOfTerrainTypes = 4;
const bool debug = true;
const std::string debug_prefix = "###: ";
const std::string faultySavesPath = "tests/faultySaves/";

// UI constants
const int kInitWindowWidth = 800;
const int kInitWindowHeight = 600;

const sf::Vector2f kClickableTextHoveredScale = sf::Vector2f(1.1f, 1.1f); // sets the default scale change when hovering over a ui::ClickableText object
const sf::Vector2f kClickableCircleShapeHoveredScale = sf::Vector2f(1.3f, 1.3f); // sets the default scale change when hovering over a ui::ClickableCircleShape object

const std::string kFontPath = "texture/times.ttf";
const std::string kBackgroundImagePath = "texture/background.jpg";
const std::string kGameTitle = "Strategy Game 3";

// Info layer constants
const float infoLayerHeight = 100.f;
const int infoLayerTextSize = 20;
const sf::Color infoLayerColor = sf::Color(25, 217, 255, 255);
const sf::Color infoLayerCardColor = sf::Color(100, 0, 0, 255);
const float infoLayerCardsMargin = 20.f;
const float infoLayerCardsWidth = kInitWindowWidth - 2 * infoLayerCardsMargin;
const float infoLayerCardHeight = kInitWindowHeight / 4.f;
const float infoLayerCardWidth = 100.f;

const std::vector<std::string> resourceTypeNames = {
    "Gold",
    "Wood",
    "Metal",
    "Food",
};
const std::vector<std::string> buildingTypeNames = {
    "Capital",
    "Farm",
    "Mine",
    "Neighborhood",
    "Power",
};
} // namespace constants