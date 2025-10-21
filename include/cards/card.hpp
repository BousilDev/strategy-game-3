#pragma once

/**
 * @file card.hpp
 * @brief Declares the Card class which represents cards played by the players. Cards 
 * construct buildings, deploy units and activate different effects.
 */

#include <string>

#include "world/tile.hpp"
#include "buildings/building.hpp"
#include "units/unit.hpp"
#include "effects/effect.hpp"

namespace cards {
  
/**
 * @brief Abstract base class representing a playable card.
 * 
 * Cards make up a deck and can be played by players during their turn from their hand.
 * A card has the virtual play method which is called along with a tile to play the 
 * card. There are different subclasses for the Card class which determine how playing
 * the card affects the tile.
 */
class Card {
public:
  /**
   * @brief Construct a new Card object.
   * 
   * @param name The name of the card.
   * @param description The description of what the card does.
   */
  Card(std::string& name, std::string& description) : name_(name), description_(description) {}

  /**
   * @brief Get the name of the card.
   * 
   * @return The name of the card.
   */
  std::string GetName();

  /**
   * @brief Get the description of the card.
   * 
   * The description of the card informs the player of what the card does.
   * 
   * @return The description of the card.
   */
  std::string GetDescription();

  /**
   * @brief Play the card.
   * 
   * This is implemented in the subclasses.
   * 
   * @param target The tile the card is played on.
   */
  virtual void Play(world::Tile& target) {}
private:
  std::string name_;
  std::string description_;
};

class BuildingCard : public Card {
public:
  /**
   * @brief Construct a new BuildingCard object.
   * 
   * @param name The name of the card.
   * @param description The description of the card.
   * @param building The building the card constructs when played.
   */
  BuildingCard(std::string& name, std::string& description, buildings::Building& building)
    : Card(name, description), building_(building) {}

  /**
   * @brief Plays the card on the tile by constructing the building of the card.
   * 
   * @param target The tile the card is played on and thus where its building is constructed.
   */
  virtual void Play(world::Tile& target);
private:
  buildings::Building building_;
};

class UnitCard : public Card {
public:
  /**
   * @brief Construct a new UnitCard object/
   * 
   * @param name The name of the card.
   * @param description The description of the card.
   * @param unit The unit the card deploys when played.
   */
  UnitCard(std::string& name, std::string& description, units::Unit& unit)
    : Card(name, description), unit_(unit) {}

  /**
   * @brief Plays the card on the tile by deploying the unit of the card.
   * 
   * @param target The tile the card is played on and thus where its unit is deployed.
   */
  virtual void Play(world::Tile& target);
private:
  units::Unit unit_;
};

class EffectCard : public Card {
public:
  /**
   * @brief Construct a new EffectCard object.
   * 
   * @param name The name of the card.
   * @param description The description of the card.
   * @param effect The effect the card causes when played.
   */
  EffectCard(std::string& name, std::string& description, effects::Effect& effect)
    : Card(name, description), effect_(effect) {}

  /**
   * @brief Plays the card on the tile by placing the effect on the tile.
   * 
   * @param target The tile the card is played on and thus where the effect is placed.
   */
  virtual void Play(world::Tile& target);
private:
  effects::Effect effect_;
};
  
} // namespace cards
