#include "tests.hpp"
#include "constants/constants.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>
#include <filesystem>

// Sends the file and line number to the AssertWithMessageFull function
#define AssertWithMessage(condition, message) AssertWithMessageFull(condition, message, __FILE__, __LINE__)

// Constructs and returns a test game instance
core::Game tests::CreateTestGame(unsigned int player_count, unsigned int map_size) {
    core::Game game;
    std::vector<core::Game::PlayerInit> players;
    std::shared_ptr<cards::Card> test_card = std::make_shared<cards::BuildingCard>("Test card", "This is a test card", core::Resource(core::ResourceType::kGold, 1), buildings::FarmBuilding::CreateEmpty(10));
    std::vector<std::shared_ptr<cards::Card>> test_cards = {};
    int card_count = 10;
    for (int i = 0; i < 10; i++) {
        test_cards.push_back(test_card->Clone());
    }
    cards::Deck test_deck = cards::Deck(test_cards, card_count);
    for (unsigned int i = 0; i < player_count; ++i) {
        players.emplace_back(core::Game::PlayerInit{
            "Player " + std::to_string(i + 1),
            test_deck.Clone() // TODO: add custom starter decks
        });
    }
    game.Initialize(players, map_size);
    return game;
}

// Testing the Game class
void tests::TestGameInitializationAndTurns() {
  std::cout << "Testing Game class..." << std::endl;
  core::Game game = CreateTestGame(4, 5);
  AssertWithMessage(game.IsInitialized() && !game.IsOver(), "Game should be initialized and not over after initialization.");
  AssertWithMessage(game.GetCurrentPlayer().GetName() == "Player 1", "Current player should be 'Player 1' after initialization.");
  for (int i = 0; i < 5; i++) {
    game.NextTurn();
    AssertWithMessage(game.GetCurrentTurn() == i + 1, "Turn number should be " + std::to_string(i + 1) + " after " + std::to_string(i) + " turns.");
  }
  for (int i = 0; i < 3; i++) {
    const std::shared_ptr<buildings::Building> capital = game.GetCurrentPlayer().GetBuildings().front();
    game.GetCurrentPlayer().RemoveBuilding(capital);
    game.NextTurn();
    AssertWithMessage(game.GetNofPlayers() == 4 - (i + 1), "Number of players should be " + std::to_string(4 - (i + 1)) + " after removing a player.");
  }
  AssertWithMessage(game.IsOver(), "Game should be over when only one player is left.");
  std::cout << "Game class tests completed successfully." << std::endl;
};

void tests::DebugGameState(const core::Game& game) {
    std::cout << "Game State Debug" << std::endl;
    core::PrintTestMsg("Is Initialized: ", game.IsInitialized());
    core::PrintTestMsg("Number of Players: ", game.GetNofPlayers());
    core::PrintTestMsg("Current Turn: ", game.GetCurrentTurn());
    core::PrintTestMsg("Current Player: ", game.GetCurrentPlayer().GetName());
}

/**
 * @brief Tests loading a game from a faulty file.
 * 
 * This function tests whether an exception is thrown when loading a game
 * from a faulty file. It opens the file, attempts to load the game from the file,
 * and checks whether the expected exception is thrown.
 * 
 * @param filename The name of the file to load from.
 */
void TestFaultyFile(std::string filename) {
    core::Game faultyGame;
    std::ifstream faultyFile(filename);
    if (faultyFile.is_open()) {
        try {
            faultyGame.Load(faultyFile);
            tests::AssertWithMessage(false, "Loading from a faulty file should fail: " + filename);
        } catch (std::exception& e) {
            core::PrintTestMsg("Correctly caught exception when loading faulty file '", filename, "': ", e.what());
        }
        faultyFile.close();
    }
}

void tests::TestGameSaveAndLoad() {
    std::cout << "Testing Game Save and Load..." << std::endl;
    core::Game game = CreateTestGame();
    // Advance a few turns
    for (int i = 0; i < game.GetNofPlayers(); i++) {
        game.NextTurn();
        // Add resources to the current player for testing
        game.GetCurrentPlayer().AddResources({core::Resource(core::ResourceType::kGold, 10 + 2 * i)});
        // Make the capital of the players take damage
        game.GetCurrentPlayer().GetBuildings().front()->takeDamage(20 * i);
        // Use the first card from the player's hand
        std::shared_ptr<cards::Card> card_to_play = game.GetCurrentPlayer().GetHand()->GetCards().front();
        game.PlayCardOnTile(card_to_play, game.GetMap().get_tile(i + 1));
        units::Unit::Create(game.GetCurrentPlayer().GetCapitalBuilding()->getTile(), game.GetCurrentPlayerPtr(), 10, units::UnitType::kSoldier);
    }
    // Save the game state
    std::ofstream outFile("testSaveFile.txt");
    if (outFile.is_open()) {
        game.Save(outFile);
        outFile.close();
    } else {
        std::cerr << "Failed to open file for saving." << std::endl;
        return;
    }
    // Test loading with a faulty files
    // Get files in tests/faultySaves/
    try {
        for (const auto& file : std::filesystem::directory_iterator(constants::faultySavesPath)) {
            if (file.is_regular_file() && file.path().extension() == ".txt") {
                TestFaultyFile(file.path().string());
            }
        }
    } catch(const std::exception& e) {
        AssertWithMessage(false, "Could not access faulty saves directory.");
    }
    
    // Load the game state into a new Game instance
    core::Game loadedGame;
    std::ifstream inFile("testSaveFile.txt");
    if (inFile.is_open()) {
        loadedGame.Load(inFile);
        inFile.close();
    } else {
        std::cerr << "Failed to open file for loading." << std::endl;
        return;
    }
    // Check that the loaded game matches the original game
    AssertWithMessage(loadedGame.IsInitialized() == game.IsInitialized(), "Loaded game initialization state should match original.");
    AssertWithMessage(loadedGame.GetNofPlayers() == game.GetNofPlayers(), "Loaded game number of players should match original.");
    AssertWithMessage(loadedGame.GetCurrentTurn() == game.GetCurrentTurn(), "Loaded game current turn should match original.");
    AssertWithMessage(loadedGame.GetCurrentPlayer().GetName() == game.GetCurrentPlayer().GetName(), "Loaded game current player name should match original.");
    // Check each player's state
    for (unsigned int i = 0; i < game.GetNofPlayers(); ++i) {
        const core::Player& originalPlayer = game.GetCurrentPlayer();
        const core::Player& loadedPlayer = loadedGame.GetCurrentPlayer();
        const auto& originalResources = originalPlayer.GetResources();
        const auto& loadedResources = loadedPlayer.GetResources();
        AssertWithMessage(originalPlayer.GetName() == loadedPlayer.GetName(), "Player names for player " + std::to_string(i + 1) + " should match.");
        AssertWithMessage(originalResources.size() == loadedResources.size(), "Resource size for player " + std::to_string(i + 1) + " should match.");
        for (size_t j = 0; j < originalResources.size(); ++j) {
            AssertWithMessage(originalResources[j].type == loadedResources[j].type, "Resource types for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalResources[j].amount == loadedResources[j].amount, "Resource amounts for player " + std::to_string(i + 1) + " should match.");
        }
        auto& originalBuildings = originalPlayer.GetBuildings();
        auto& loadedBuildings = loadedPlayer.GetBuildings();
        const auto& originalBuildingsVector = std::vector<std::shared_ptr<buildings::Building>>(originalBuildings.begin(), originalBuildings.end());
        const auto& loadedBuildingsVector = std::vector<std::shared_ptr<buildings::Building>>(loadedBuildings.begin(), loadedBuildings.end());
        for (size_t j = 0; j < originalBuildings.size(); ++j) {
            const auto& originalBuilding = originalBuildingsVector[j];
            const auto& loadedBuilding = loadedBuildingsVector[j];
            AssertWithMessage(originalBuilding->GetType() == loadedBuilding->GetType(), "Building types for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalBuilding->getMaxHp() == loadedBuilding->getMaxHp(), "Building max HP for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalBuilding->getCurrentHp() == loadedBuilding->getCurrentHp(), "Building current HP for player " + std::to_string(i + 1) + " should match.");
        }
        auto& originalUnits = originalPlayer.GetUnits();
        auto& loadedUnits = loadedPlayer.GetUnits();
        const auto& originalUnitsVector = std::vector<std::shared_ptr<units::Unit>>(originalUnits.begin(), originalUnits.end());
        const auto& loadedUnitsVector = std::vector<std::shared_ptr<units::Unit>>(loadedUnits.begin(), loadedUnits.end());
        for (size_t j = 0; j < originalUnits.size(); ++j) {
            const auto& originalUnit = originalUnitsVector[j];
            const auto& loadedUnit = loadedUnitsVector[j];
            AssertWithMessage(originalUnit->GetType() == loadedUnit->GetType(), "Unit types for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalUnit->getMaxHp() == loadedUnit->getMaxHp(), "Unit max HP for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalUnit->getCurrentHp() == loadedUnit->getCurrentHp(), "Unit current HP for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(loadedPlayer.GetCapitalBuilding()->getTile()->get_unit() != nullptr, "There should be a unit on the capital building tile.");
        }
        game.NextTurn();
        loadedGame.NextTurn();
    }
    constants::debug ? DebugGameState(loadedGame) : void();
    std::cout << "Game Save and Load tests completed successfully." << std::endl;
}