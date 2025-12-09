#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>

/**
 * @file constants.hpp
 * @brief Defines the Constants namespace which declares all global constants.
 */

namespace constants {
const int kNumberOfResourceTypes = 4;
const int kNumberOfTerrainTypes = 4;
const bool debug = false;
const std::string debug_prefix = "###: ";
const std::string faultySavesPath = "tests/faultySaves/";
const std::string kSavesPath = "saves/";
const std::filesystem::path kFramesPath = "texture/frames";
const std::filesystem::path kDelaysFile = kFramesPath / "delays_cs.txt";

// -----UI constants-----
const int kInitWindowWidth = 800;
const int kInitWindowHeight = 600;
const bool kUseCoolTitle = true; // true to use the cool title animation in the main menu, false to use static text

// Main menu clickable elements (ui::ClickableText, ui::ClickableCircleShape and ui::Selection)
const int kMainMenuClickablesSize = 40;
const sf::Color kClickableNormalColor = sf::Color(255, 255, 255, 255);              // normal color
const sf::Color kClickableHoveredColor = sf::Color(200, 150, 150, 255);             // hovered color
const float kClickableOutlineSize = 1.f;                                            // outline size
const sf::Color kClickableOutlineColor = sf::Color(50, 0, 0, 255);                 // outline color
const sf::Vector2f kClickableTextHoveredScale = sf::Vector2f(1.1f, 1.1f);           // ui::ClickableText hovered scale
const sf::Vector2f kClickableCircleShapeHoveredScale = sf::Vector2f(1.3f, 1.3f);    // ui::ClickableCircleShape hovered scale
const sf::Color kSelectionTextOutlineColor = sf::Color(50, 0, 0, 255);             // ui::Selection outline color
const float kSelectionTextOutlineSize = 1.f;                                        // ui::Selection outline size

// ui::FileSelection
const int kFileSelectionFontSize = 30;
const sf::Color kFSselectedTextColor = sf::Color(255, 0, 0, 255);   // ui::FileSelection selected text color
const sf::Color kFSselectedTextOutline = sf::Color(127, 0, 0, 255); // ui::FileSelection selected text outline color
const sf::Color kFStextColor = sf::Color(255, 255, 255, 255);       // ui::FileSelection default text color
const sf::Color kFSbgColor = sf::Color(50, 50, 50, 100);            // ui::FileSelection background color
const sf::Color kFSoutlineColor = sf::Color(255, 255, 255, 200);    // ui::FileSelection outline color
const float kFSmargin = 0.2f;                                       // ui::FileSelection margin size compared to window size
const sf::Color kScrollbarTrackColor = sf::Color(60, 60, 60, 120);
const sf::Color kScrollbarThumbColor = sf::Color(170, 170, 170, 255);
const float kScrollbarPadding = 4.f;    // ui::FileSelection scrollbar gap from right edge of background_
const float kScrollbarWidth = 8.f;      // ui::FileSelection scrollbar track width
const float kMinThumbHeight = 24.f;     // ui::FileSelection scrollbar minimum thumb height
const float kFScontentPadding = 8.f;    // ui::Fileselection text padding inside the element

// ui::TextInput
const size_t TextInputMaxLength = 64; // Max length for the text input buffer
const float TextInputCaretTime = 0.f;
const float TextInputCaretBlinkPeriod = 0.5f; // seconds
const sf::Color kTextInputTextColor = sf::Color(255, 255, 255, 255);
const sf::Color kTextInputTextOutlineColor = sf::Color(100, 0, 0, 255);
const float kTextInputTextOutlineSize = 1.f;

const std::string kFontPath = "texture/times.ttf";
const std::string kBackgroundImagePath = "texture/background.jpg";
const std::string kGameTitle = "Strategy Game 3";

// Map display constants
const float mapPanSpeed = 3;
const int viewDistance = 2;
const sf::Color playerOneColor = sf::Color(233,20,20);
const sf::Color playerTwoColor = sf::Color(20,233,20);
const sf::Color playerThreeColor = sf::Color(20,20,233);
const sf::Color playerFourColor = sf::Color(233,233,20);

// Info layer constants
const float infoLayerHeight = 100.f;
const int infoLayerTextSize = 20;
const sf::Color infoLayerColor = sf::Color(25, 217, 255, 255);
const sf::Color infoLayerCardColor = sf::Color(100, 0, 0, 255);
const float infoLayerCardsMargin = 20.f;
const float infoLayerCardsWidth = kInitWindowWidth - 2 * infoLayerCardsMargin;
const float infoLayerCardHeight = kInitWindowHeight / 4.f;
const float infoLayerCardWidth = kInitWindowWidth / 6.f;
const float infoLayerCardTextSize = infoLayerTextSize * 2 / 3.f;

const std::vector<std::string> resourceTypeNames = {
    "Gold",
    "Wood",
    "Metal",
    "Food",
};
const std::vector<std::string> buildingTypeNames = {
    "Capital",
    "Farm",
    "LumberMill",
    "Mine"
};

const std::vector<std::string> unitTypeNames = {
    "Soldier",
};

const std::vector<std::string> cardsTypeNames = {
    "BuildingCard",
    "UnitCard"
};

} // namespace constants