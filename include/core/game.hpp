#pragma once

/**
 * @file game.hpp
 * @brief Declares the Game class which manages the main game state and flow.
 */

#include <memory>
#include <string>
#include <vector>

#include "core/player.hpp"
#include "world/map.hpp"
#include "cards/deck.hpp"

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
  Game() : nof_players_(0), current_turn_(0), turn_(0), is_initialized_(false) {};

  struct PlayerInit {
    std::string name;
    std::shared_ptr<cards::Deck> deck;
  };

  /**
   * @brief Sets the debug flag.
   * 
   * @param debug The debug flag.
   */
  void SetDebug(bool debug) { debug_ = debug; }

  /**
   * @brief Initializes the game according to the given parameters.
   * 
   * @param players The names and decks of the players in the game.
   * @param map_size The size of the map as an integer. The map will contain 
   * map_size * map_size tiles.
   */
  void Initialize(const std::vector<PlayerInit>& players, unsigned int map_size);


  /**
   * @brief Saves the state of the game.
   * Saves the state of the map and the states of the players along with the
   * state of this class into a text file. If the file already contains data, it
   * will be erased. An uninitialized game cannot be saved.
   *
   * @param filename The name of the file to save to.
   */
  void Save(std::ostream& out) const;

  /**
   * @brief Loads a game from a file.
   * Uses the same formatting as saving a game. Assumes the file only contains data for
   * a single game.
   * 
   * @param filename The name of the file with a game to load.
   */
  void Load(std::istream& in);

  /**
   * @brief Checks if the game is over.
   * The game is over when only one player is left alive.
   * 
   * @return true if the game is over and false if not.
   */
  bool IsOver() const;

  /**
   * @brief Tells if the game has been initialized.
   * 
   * @return true if the game has been initialized and false if not.
   */
  bool IsInitialized() const { return is_initialized_; }

  /**
   * @brief Advances to next turn and updates alive players.
   * Updates alive and dead players and current player. If the capital building of an alive 
   * player was destroyed, this player is now dead. Therefore, they have no more turns to 
   * play. 
   */
  void NextTurn();

  /**
   * @brief Get the Map of the game.
   * 
   * @return A reference to the world map.
   */
  const world::Map& GetMap() const { return map_; }

  /**
   * @brief Get the player whose turn it is.
   * 
   * @return A reference to the current player.
   */
  Player& GetCurrentPlayer() { return *players_[current_turn_]; }

  /**
   * @brief Get the player whose turn it is.
   * 
   * @return A const reference to the current player.
   */
  const Player& GetCurrentPlayer() const { return *players_[current_turn_]; }

  /**
   * @brief Get the number of players in the game.
   * 
   * @return The number of players in the game.
   */
  unsigned int GetNofPlayers() const { return nof_players_; }

  /**
   * @brief Get the current turn.
   * 
   * @return The current turn.
   */
  unsigned int GetCurrentTurn() const { return turn_; }

  /**
   * @brief Plays a card on a tile for the current player.
   * 
   * @param card The card to be played.
   * @param tile The tile the card is played on.
   */
  void PlayCardOnTile(std::shared_ptr<cards::Card> card, std::shared_ptr<world::Tile> tile);

private:
  std::vector<std::shared_ptr<Player>> players_; ///< All players in the game.
  std::vector<std::shared_ptr<Player>> dead_players_; ///< All players that have lost the game.
  unsigned int nof_players_; ///< The number of players in the game.
  world::Map map_; ///< The map of the game.
  std::vector<std::shared_ptr<world::Tile>> spawn_tiles_;
  unsigned int current_turn_; ///< The index of the current player in players_.
  unsigned int turn_; ///< Global turn count.
  bool is_initialized_; ///< Indicator for whether the game has been initialized.
  bool debug_ = constants::debug; ///< If true, debug information is printed to the console.
};

} // namespace core
