#pragma once

/**
 * @file Game.hpp
 * @brief Declares the Game class which manages the main game state and flow.
 */

#include <memory>
#include <string>
#include <vector>

#include "core/Player.hpp"
#include "world/Map.hpp"

namespace core {

/**
 * @class Game
 * @brief Central class which manages starting and ending a game, turn order and world map.
 * 
 * The Game class controls the main flow of a game. It initializes, loads and saves games,
 * and advances turns. It stores the players both alive and dead and knows who's turn it is.
 * When a game has only one player alive, it is over.
 * 
 * Example usage:
 * @code
 * core::Game game;
 * game.initialize();
 * Player current_player = game.getCurrentPlayer();
 * @endcode
 */
class Game {
public:
  /**
   * @brief Construct an empty game instance.
   */
  Game();

  /**
   * @brief Initializes the game according to the given parameters.
   * 
   * @param player_count The number of players in the game.
   * @param player_names The names of the players. Must be of equal length to the number of 
   * players.
   * @param map_size The size of the map as an integer. The map will contain 
   * map_size * map_size tiles.
   * @return true if the initialization was successful and false if it failed.
   */
  bool initialize(int player_count, std::vector<std::string>& player_names, int map_size);

  /**
   * @brief Saves the state of the game. 
   * Saves the state of the map and the states of the players along with the state of this
   * class into a text file. If the file already contains data, it will be erased. An 
   * uninitialized game cannot be saved.
   * 
   * @param filename The name of the file to save to.
   * @return true if saving was successful and false if it failed.
   */
  bool save(std::string filename);

  /**
   * @brief Loads a game from a file.
   * Uses the same formatting as saving a game. Assumes the file only contains data for
   * a single game.
   * 
   * @param filename The name of the file with a game to load.
   * @return true if loading the game was successful and false if it failed.
   */
  bool load(std::string filename);

  /**
   * @brief Checks if the game is over.
   * The game is over when exactly one player is left alive.
   * 
   * @return true if the game is over and false if not.
   */
  bool isOver();

  /**
   * @brief Advances to next turn and updates alive players.
   * Updates alive and dead players and current player. If the capital building of an alive 
   * player was destroyed, this player is now dead. Therefore, they have no more turns to 
   * play. 
   */
  void nextTurn();

  /**
   * @brief Get the Map of the game.
   * 
   * @return A reference to the world map.
   */
  world::Map& getMap();

  /**
   * @brief Get the player whose turn it is.
   * 
   * @return A reference to the current player.
   */
  Player& getCurrentPlayer() { return *players_[current_turn_]; }

  /**
   * @brief Get all the players in the game.
   * 
   * @return A vector of pointers to the players.
   */
  std::vector<std::shared_ptr<Player>> getPlayers() { return players_; }

  /**
   * @brief Get the players still in the game.
   * 
   * @return A reference to a vector of pointers to the players.
   */
  std::vector<std::shared_ptr<Player>>& getAlivePlayers();

  /**
   * @brief Get the players who have been defeated.
   * 
   * @return A reference to a vector of pointers to the players.
   */
  std::vector<std::shared_ptr<Player>>& getDeadPlayers();

private:
  std::vector<std::shared_ptr<Player>> players_; ///< All players in the game.
  int current_turn_; ///< The index of the current player in players_.
  int turn_; ///< The number of the current turn.
  bool is_initialized_; ///< Indicator for whether the game has been initialized.
};

} // namespace core