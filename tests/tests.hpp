#pragma once

#include "core/game.hpp"
#include "core/utils.hpp"
#include <iostream>

namespace tests {
/**
 * @brief Throws a runtime error if the condition is false, with a message including the file and line number.
 * 
 * This function is used to assert conditions in tests. It is equivalent to calling
 * core::ThrowWithMessage with the file and line number obtained from the __FILE__ and __LINE__
 * macros.
 * @param condition The condition to check.
 * @param message The message to display if the condition is false.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 */
  inline void AssertWithMessageFull(bool condition, const std::string& message, const std::string& file, int line) {
    if (!condition) {
      core::ThrowWithMessage(message, file, line);
    }
  }
  
  /**
 * @brief Creates a test game instance.
 * 
 * Creates a game instance with the given number of players and map size. Each player
 * is given a deck with test cards. The game is then
 * initialized with the given parameters.
 * 
 * @param player_count The number of players in the test game.
 * @param map_size The size of the map in the test game.
 * @return A game instance with the given parameters.
 */
  core::Game CreateTestGame(unsigned int player_count = 4, unsigned int map_size = 5);

  /**
 * @brief Tests the Game class's initialization and turn advancing.
 * 
 * Checks that after initialization, the game is initialized and not over.
 * Then checks that the current player is 'Player 1', and that the turn number
 * increases by one after each call to NextTurn(). Then checks that the number of
 * players decreases by one after each call to NextTurn() after removing a capital building.
 * Finally checks that the game is over when only one player is left.
 */
  void TestGameInitializationAndTurns();

  // Debugs the current state of the game by printing relevant information to the console.
  void DebugGameState(const core::Game& game);

  /**
 * @brief Tests saving and loading a game state.
 * 
 * This function tests the Game class's Save and Load methods by saving
 * a game state to a file and then loading it back into a new Game instance.
 * It checks that the loaded game matches the original game by comparing
 * the game state (initialization, number of players, current turn, and current
 * player) and the state of each player (resources, buildings, and units).
 */
  void TestGameSaveAndLoad();

  /**
 * @brief Tests the Player class.
 * 
 * This function tests the Player class by adding and removing resources, and testing the IsAlive method.
 */
  void TestPlayer();

  /**
 * @brief Tests the Map class which manages the game map.
 * 
 * Tests the size of the map, that each tile is not null, and that each tile has a valid terrain type.
 * Also tests that the capital building is correctly placed on its tile and that trying to place another building on the same tile does not work.
 */
  void TestMap();

  /**
 * @brief Tests the creation and destruction of buildings.
 * 
 * This function tests the creation and destruction of buildings. It creates
 * a farm building and tests its properties, then applies damage and tests
 * the resulting HP. Finally, it destroys the building and tests that the
 * tile is left empty.
 */
  void TestBuildings();

  /**
 * @brief Test the Unit class.
 *
 * This function tests the creation and damage application of Unit objects.
 * It also tests the attacking functionality of Unit objects.
 */
  void TestUnits();

  /**
 * @brief Unit tests for the cards module.
 * 
 * This function tests the creation of a BuildingCard, its properties and its cloning.
 */
  void TestCards();
}