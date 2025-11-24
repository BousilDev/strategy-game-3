#pragma once

/**
 * @file card.hpp
 * @brief Declares the Card class which represents cards played by the players. Cards 
 * construct buildings, deploy units and activate different effects.
 */

#include <string>
#include <memory>

#include "buildings/building.hpp"
#include "effects/effect.hpp"
#include "world/tile.hpp"
#include "units/unit.hpp"


namespace cards {

/**
 * @brief Enumerator for different card types
 */
enum class CardType { kBuilding, kUnit, kEffect };
  
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
  Card(std::string name, std::string description) : name_(name), description_(description) {}

  /**
   * @brief Destroy the Card object.
   */
  virtual ~Card() = default;

  /**
   * @brief Clone the card.
   * 
   * @return A copy of the card.
   */
  virtual std::shared_ptr<Card> Clone() const = 0;

  /**
   * @brief Get the name of the card.
   * 
   * @return The name of the card.
   */
  const std::string& GetName() const noexcept { return name_; }

  /**
   * @brief Get the description of the card.
   * 
   * The description of the card informs the player of what the card does.
   * 
   * @return The description of the card.
   */
  const std::string& GetDescription() const noexcept { return description_; }

  /**
   * @brief Get the type of the card.
   * 
   * @return The type of the card.
   */
  virtual CardType GetCardType() const = 0;

  /**
   * @brief Play the card if the target allows the card to be played on it.
   * If the card is succesfully played, the hand will discard the card.
   * 
   * This is implemented in the subclasses. 
   * 
   * @param target The tile the card is played on.
   * @param player The player who is playing the card.
   * @return true If the card was played and false otherwise. 
   */
  virtual bool Play(world::Tile& target, std::shared_ptr<core::Player> player) = 0;

private:
  std::string name_; ///< The name of the card.
  std::string description_; ///< The description of the card.
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
  BuildingCard(std::string name, std::string description, const std::shared_ptr<buildings::Building> building)
    : Card(name, description), building_(building->CreateEmptyFromCopy()) {}

  /**
   * @brief Copy constructor for the BuildingCard.
   * 
   * @param other Other BuildingCard
   */
  BuildingCard(const BuildingCard& other)
    : Card(other),
      building_(other.building_ ? other.building_->CreateEmptyFromCopy() : nullptr) {}

  /**
   * @brief Clone the card.
   * 
   * @return A copy of the card.
   */
  std::shared_ptr<Card> Clone() const override { return std::make_shared<BuildingCard>(*this); }

  /**
   * @brief Get the type of the card (kBuilding).
   * 
   * @return CardType::kBuilding
   */
  CardType GetCardType() const override { return CardType::kBuilding; }
  
  /**
   * @brief Plays the card by constructing a copy its building on the target tile if possible.
   * If the card is succesfully played, the hand will discard the card.
   * 
   * @param target The tile the card is played on and thus where its building is constructed.
   * @param player The player who is playing the card.
   * @return true If the building was successfully constructed on the target tile, false otherwise.
   */
  bool Play(world::Tile& target, std::shared_ptr<core::Player> player) override;

private:
  std::shared_ptr<buildings::Building> building_; ///< The building the card constructs a copy of.
};

class UnitCard : public Card {
public:
  /**
   * @brief Construct a new UnitCard object.
   * 
   * @param name The name of the card.
   * @param description The description of the card.
   * @param unit The unit the card deploys when played.
   */
  UnitCard(std::string name, std::string description, const std::shared_ptr<units::Unit> unit)
    : Card(name, description), unit_(unit->CreateEmptyFromCopy()) {}

  /**
   * @brief Copy constructor for the UnitCard.
   * 
   * @param other Other UnitCard
   */
  UnitCard(const UnitCard& other)
    : Card(other),
      unit_(other.unit_ ? other.unit_->CreateEmptyFromCopy() : nullptr) {}

  /**
   * @brief Clone the card.
   * 
   * @return A copy of the card.
   */
  std::shared_ptr<Card> Clone() const override { return std::make_shared<UnitCard>(*this); }

  /**
   * @brief Get the type of the card (kUnit).
   * 
   * @return CardType::kUnit
   */
  CardType GetCardType() const override { return CardType::kUnit; }
  
  /**
   * @brief Plays the card by deploying a copy of its unit on the target tile.
   * If the card is succesfully played, the hand will discard the card.
   * 
   * @param target The tile the card is played on and thus where its unit is deployed.
   * @param player The player who is playing the card.
   * @return true If the unit was successfully deployed on the target tile, false otherwise.
   */
  bool Play(world::Tile& target, std::shared_ptr<core::Player> player) override;

private:
  std::shared_ptr<units::Unit> unit_; ///< The unit the card deploys a copy of.
};

// TODO: DECIDE WHETHER TO INCLUDE

// class EffectCard : public Card {
// public:
//   /**
//    * @brief Construct a new EffectCard object.
//    * 
//    * @param name The name of the card.
//    * @param description The description of the card.
//    * @param effect The effect the card causes when played.
//    */
//   EffectCard(std::string name, std::string description, const std::shared_ptr<effects::Effect> effect);

//   /**
//    * @brief Copy constructor for the EffectCard.
//    * 
//    * @param other Other EffectCard
//    */
//   EffectCard(const EffectCard& other);
//   // : Card(other),
//   //   effect_(other.effect_ ? other.effect_->Clone() : nullptr) {}

//   /**
//    * @brief Clone the card.
//    * 
//    * @return A copy of the card.
//    */
//   std::shared_ptr<Card> Clone() const override { return std::make_shared<EffectCard>(*this); }

//   /**
//    * @brief Get the type of the card (kEffect).
//    * 
//    * @return CardType::kEffect
//    */
//   CardType GetCardType() const override { return CardType::kEffect; }

//   /**
//    * @brief Plays the card by applying a copy of its effect to the target tile.
//    * If the card is succesfully played, the hand will discard the card. 
//    *
//    * @param target The tile the card is played on and thus where the effect is placed.
//    * @return true If the effect was successfully applied on the target tile, false otherwise.
//    */
//   bool Play(world::Tile& target) override;

// private:
//   std::shared_ptr<effects::Effect> effect_; ///< The effect the card places a copy of.
// };
  
} // namespace cards
