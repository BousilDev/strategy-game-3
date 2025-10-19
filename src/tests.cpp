#include "core/tests.hpp"
#include "constants/constants.hpp"
#include <iostream>
#include <assert.h>

// Test prefix wrapper
template <typename T>
void core::PrintTestMsg(const T& msg) {
  std::cout << constants::debug_prefix << msg;
}
void core::PrintTestMsg(const std::ostringstream& msg) {
  PrintTestMsg(msg.str());
}

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