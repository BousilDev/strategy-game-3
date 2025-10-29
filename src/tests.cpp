#include "core/tests.hpp"
#include "constants/constants.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>

// Testing the Game class
void core::TestGameInitializationAndTurns() {
  std::cout << "Testing Game class..." << std::endl;
  unsigned int map_size = 5;
  Game game;
  const std::vector<Game::PlayerInit> players = {
    {"Test Gamer", cards::Deck()},
    {"Bob the Builder", cards::Deck()},
    {"Jari the Destroyer", cards::Deck()},
    {"Markku the Conqueror", cards::Deck()}
  };
  game.Initialize(players, map_size);
  assert(game.IsInitialized() && !game.IsOver());
  assert(game.GetCurrentPlayer().GetName() == "Test Gamer");
  for (int i = 0; i < 5; i++) {
    game.NextTurn();
    assert(game.GetCurrentTurn() == i + 1);
  }
  for (int i = 0; i < 3; i++) {
    const std::shared_ptr<buildings::Building> capital = game.GetCurrentPlayer().GetBuildings().front();
    game.GetCurrentPlayer().RemoveBuilding(capital);
    game.NextTurn();
    assert(game.GetNofPlayers() == 4 - (i + 1));
  }
  assert(game.IsOver());
  std::cout << "Game class tests completed successfully." << std::endl;
};

void core::DebugGameState(const Game& game) {
    std::cout << "Game State Debug" << std::endl;
    core::PrintTestMsg("Is Initialized: ", game.IsInitialized());
    core::PrintTestMsg("Number of Players: ", game.GetNofPlayers());
    core::PrintTestMsg("Current Turn: ", game.GetCurrentTurn());
    core::PrintTestMsg("Current Player: ", game.GetCurrentPlayer().GetName());
    // Print each player's resources
    for (unsigned int i = 0; i < game.GetNofPlayers(); ++i) {
        const core::Player& player = game.GetCurrentPlayer();
        core::PrintTestMsg("- Player ", i + 1, " (", player.GetName(), ") Resources:");
        for (const auto& resource : player.GetResources()) {
            core::PrintTestMsg("- - Resource Type: ", static_cast<int>(resource.type), ", Amount: ", resource.amount);
        }
    }
}

void core::TestGameSaveAndLoad() {
    std::cout << "Testing Game Save and Load..." << std::endl;
    unsigned int map_size = 5;
    Game game;
    std::vector<core::Game::PlayerInit> players;
    unsigned int player_count = 4;
    for (unsigned int i = 0; i < player_count; ++i) {
        players.emplace_back(core::Game::PlayerInit{
            "Player " + std::to_string(i + 1),
            cards::Deck() // TODO: add custom starter decks
        });
    }
    game.Initialize(players, map_size);
    // Advance a few turns
    for (int i = 0; i < 3; i++) {
        game.NextTurn();
        // Add resources to the current player for testing
        game.GetCurrentPlayer().AddResources({core::Resource(core::ResourceType::kGold, 10 + 2 * i)});
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
    // Load the game state into a new Game instance
    Game loadedGame;
    std::ifstream inFile("testSaveFile.txt");
    if (inFile.is_open()) {
        loadedGame.Load(inFile);
        inFile.close();
    } else {
        std::cerr << "Failed to open file for loading." << std::endl;
        return;
    }
    // Check that the loaded game matches the original game
    assert(loadedGame.IsInitialized() == game.IsInitialized());
    assert(loadedGame.GetNofPlayers() == game.GetNofPlayers());
    assert(loadedGame.GetCurrentTurn() == game.GetCurrentTurn());
    assert(loadedGame.GetCurrentPlayer().GetName() == game.GetCurrentPlayer().GetName());
    // Check resources of each player
    for (unsigned int i = 0; i < game.GetNofPlayers(); ++i) {
        const core::Player& originalPlayer = game.GetCurrentPlayer();
        const core::Player& loadedPlayer = loadedGame.GetCurrentPlayer();
        const auto& originalResources = originalPlayer.GetResources();
        const auto& loadedResources = loadedPlayer.GetResources();
        assert(originalResources.size() == loadedResources.size());
        for (size_t j = 0; j < originalResources.size(); ++j) {
            assert(originalResources[j].type == loadedResources[j].type);
            assert(originalResources[j].amount == loadedResources[j].amount);
        }
        //game.NextTurn();
        //loadedGame.NextTurn();
    }
    constants::debug ? DebugGameState(loadedGame) : void();
    std::cout << "Game Save and Load tests completed successfully." << std::endl;
}