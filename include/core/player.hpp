#pragma once

/**
 * @file Player.hpp
 * @brief Declares the Player class which manages the actions performed by a single player.
 */

#include <array>
#include <list>
#include <memory>
#include <string>

#include "buildings/building.hpp"
#include "cards/card.hpp"
#include "cards/deck.hpp"
#include "cards/hand.hpp"
#include "constants/constants.hpp"
#include "core/resource.hpp"
#include "units/unit.hpp"
#include "world/tile.hpp"

namespace core {

/**
 * @brief Central class representing a player of the game.
 * 
 * A player has a deck, a list of buildings they own and a list of units they own. A player
 * can play cards from their current hand, move units and use units. A player can also end
 * their turn.
 * 
 * Example usage:
 * @code
 * core::Player p1 = Player("p1", some_deck);
 * p1.drawHand();
 * cards::Hand hand = p1.getHand();
 * p1.playCard(hand[0]);
 * @endcode
 */
class Player {
public:

  /**
   * @brief Construct a new Player object
   * 
   * @param name The name of the player.
   * @param deck The starting deck of the player.
   */
  Player(const std::string& name, cards::Deck deck) :
    name_(name), deck_(deck), resources_({}), buildings_({}), units_({}) {};

  /**
   * @brief Get the name of the player.
   * 
   * @return The name of the player.
   */
  const std::string& GetName() const { return name_; }

  /**
   * @brief Get the current deck of the player.
   * 
   * @return The deck of the player.
   */
  cards::Deck& GetDeck() { return deck_; };

  /**
   * @brief Get the current hand of the player stored in the player's deck.
   * 
   * @return The hand of the player.
   */
  cards::Hand& GetHand() { return deck_.GetHand(); };

  /**
   * @brief Get the buildings owned by the player.
   * 
   * @return List of the buildings owned by the player.
   */
  const std::list<std::shared_ptr<buildings::Building>>& GetBuildings() const { return buildings_; }

  /**
   * @brief Get the units owned by the player.
   * 
   * @return List of the units owned by the player.
   */
  const std::list<std::shared_ptr<units::Unit>>& GetUnits() const { return units_; }

  /**
   * @brief Get the resources the player currently produces per turn.
   * 
   * Resource are produced by buildings every turn. Effects on the tiles the buildings
   * are on may affect the resources produced.
   * 
   * @return The resources produced by the player per turn.
   */
  const std::array<Resource, constants::kNumberOfResourceTypes>& GetResourcesPerTurn() const;

  /**
   * @brief Get the resources the player currently has stored.
   * 
   * @return The player's resources.
   */
  const std::array<Resource, constants::kNumberOfResourceTypes>& GetResources() const { return resources_; }

  /**
   * @brief Adds the given building to the buildings owned by the player.
   * 
   * @param building The building to be added
   */
  void AddBuilding(std::shared_ptr<buildings::Building> building);

  /**
   * @brief Removes the given building from the buildings owned by the player if owned.
   * 
   * @param building The building to be removed.
   */
  void RemoveBuilding(std::shared_ptr<buildings::Building> building);

  /**
   * @brief Adds the given unit to the units owned by the player.
   * 
   * @param unit The unit to be added.
   */
  void AddUnit(std::shared_ptr<units::Unit> unit);

  /**
   * @brief Removes the given unit from the units owned by the player if owned.
   * 
   * @param unit The unit to be removed.
   */
  void RemoveUnit(std::shared_ptr<units::Unit> unit);

  /**
   * @brief Adds the given resources to the players resources.
   * 
   * @param resources The list of resources to be added.
   */
  void AddResources(const std::list<Resource>& resources);

  /**
   * @brief Removes the amount of the resource from the player's corresponding resource.
   * 
   * @param Resource The resource to be subtracted.
   */
  void RemoveResource(Resource resource);

  /**
   * @brief Use the deck's drawHand() to draw a new hand which is stored in the deck.
   * 
   * This is used at the start of the turn to get a new set of cards for the turn.
   */
  void DrawHand() { deck_.drawHand(); };

  /**
   * @brief Checks if the player is alive or not i.e. if the capital building of the player
   * is still standing.
   * 
   * @return true if the player has a capital building and is therefore alive, and false 
   * otherwise
   */
  bool IsAlive() const;

  /**
   * @brief Play the given card on the given tile if the player has enough resources.
   * 
   * Takes a card and a tile and tries to play the card on the tile. Playing fails if
   * the player doesn't have enough resources, or the card is a building card and the tile
   * already has a building, or the card is a unit card and the tile already has a unit.
   * If playing the card is successful, subtracts the resources playing the card takes from
   * the player's resources.
   * 
   * @param card The card to be possibly played.
   * @param tile The target tile to be effected by the card.
   * @return true if the card was successfully played and false otherwise.
   */
  bool PlayCard(cards::Card& card, world::Tile& tile);

  /**
   * @brief Move a unit if it is owned by the player.
   *  
   * The movement of the unit is handled by the unit itself and not the player.
   * 
   * @param unit The unit to be potentially moved.
   * @param target The target tile to move the unit to.
   */
  void MoveUnit(std::shared_ptr<units::Unit> unit, world::Tile& target);

  /**
   * @brief Use a unit if it is owned by the player.
   * 
   * The use of the unit is handled by the unit itself and not the player.
   * 
   * @param unit The unit to be potentially used.
   * @param target The target tile to use the unit on.
   */
  void UseUnit(std::shared_ptr<units::Unit> unit, world::Tile& target); 

private:
  const std::string name_; ///< The name of the player.
  cards::Deck deck_; ///< The current deck of the player.
  std::list<std::shared_ptr<buildings::Building>> buildings_; ///< The buildings owned by the player.
  std::list<std::shared_ptr<units::Unit>> units_; ///< The units owned by the player.
  std::array<Resource, constants::kNumberOfResourceTypes> resources_; ///< The resources the player currently has.
};

} // namespace core
