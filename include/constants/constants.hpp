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
const std::string kSavesPath = "saves/";

// UI constants
const int kInitWindowWidth = 800;
const int kInitWindowHeight = 600;

const int kMainMenuClickablesSize = 40;
const int kFileSelectionFontSize = 30;
const sf::Color kFSselectedTextColor = sf::Color(255, 0, 0, 255);   // ui::FileSelection selected text color
const sf::Color kFSselectedTextOutline = sf::Color(127, 0, 0, 255); // ui::FileSelection selected text outline color
const sf::Color kFStextColor = sf::Color(255, 255, 255, 255);       // ui::FileSelection default text color
const sf::Color kFSbgColor = sf::Color(50, 50, 50, 100);            // ui::FileSelection background color
const sf::Color kFSoutlineColor = sf::Color(255, 255, 255, 200);    // ui::FileSelection outline color
const float kFSmargin = 0.2f;                                       // ui::FileSelection margin size compared to window size

const std::string kFontPath = "texture/times.ttf";
const std::string kBackgroundImagePath = "texture/background.jpg";
const std::string kGameTitle = "Strategy Game 3";

// Map display constants
const float mapPanSpeed = 3;
const sf::Color playerOneColor = sf::Color(255,10,10);
const sf::Color playerTwoColor = sf::Color(10,255,10);
const sf::Color playerThreeColor = sf::Color(10,10,255);
const sf::Color playerFourColor = sf::Color(10,255,255);

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