#pragma once

#include "world/tile.hpp"
#include "core/player.hpp"
#include <memory>

/**
 * @file unit.hpp
 * @brief Declares the Unit class, representing a deployable and movable entity 
 * on the game map. Units are owned by players and can attack, and 
 * interact with other map objects such as buildings and other units.
 */

namespace units {

/**
 * @enum UnitType
 * @brief Enumerates the different types of units that can exist in the game.
 */
enum class UnitType {
  kSoldier, ///< Basic combat unit capable of attacking
};

/**
 * @class Unit
 * @brief Represents a single unit on the game map.
 *
 * The Unit class models a game entity that belongs to a player and occupies 
 * a specific tile on the map. Units can move between tiles, take and deal damage, 
 * and perform end-of-turn actions. 
 *
 * This class uses `std::enable_shared_from_this` to allow safe creation of 
 * `std::shared_ptr<Unit>` references to itself, enabling it to register with 
 * the tile and player that own it.
 *
 * To prevent circular ownership (memory leaks), the Unit holds **weak references**
 * to its current tile and owning player.
 */
class Unit : public std::enable_shared_from_this<Unit> {
public:
  /**
   * @brief Virtual destructor for safe polymorphic deletion.
   */
  virtual ~Unit() = default;

  /**
   * @brief Factory method to create and register a new Unit instance.
   *
   * This static method ensures that the created Unit is properly managed by 
   * a `std::shared_ptr`, and automatically registers it with the given 
   * tile and player (if they are not null).
   *
   * @param tile The tile the unit is placed on. Can be nullptr.
   * @param owner The player that owns this unit. Can be nullptr.
   * @param max_hp The maximum health points of the unit.
   * @param unit_type The type of the unit (e.g., soldier).
   * @return A shared pointer to the newly created Unit.
   */
  static std::shared_ptr<Unit> Create(std::shared_ptr<world::Tile> tile,
                                      std::shared_ptr<core::Player> owner,
                                      int max_hp,
                                      UnitType unit_type);

  /**
   * @brief Returns the type of this unit.
   * @return The UnitType enumeration value representing this unit's type.
   */
  UnitType GetType() const { return unit_type_; }

  /**
   * @brief Returns the maximum HP (hit points) of this unit.
   * @return The maximum HP.
   */
  int getMaxHp() const { return max_hp_; }

  /**
   * @brief Returns the current HP of this unit.
   * @return The current HP.
   */
  int getCurrentHp() const { return current_hp_; }

  /**
   * @brief Reduces the unit's HP by the specified amount.
   *
   * Ensures HP does not drop below zero.
   *
   * @param damage The amount of damage to apply.
   * @return The remaining HP after taking damage.
   */
  int takeDamage(int damage);

  /**
   * @brief Moves this unit to a new tile on the map.
   *
   * This function removes the unit from its current tile (if any) 
   * and places it onto the specified new tile.
   *
   * @param tile The destination tile to move this unit to.
   * @return True if the move was successful, false if the tile was occupied or invalid.
   */
  bool moveToTile(std::shared_ptr<world::Tile> tile);

  /**
   * @brief Deals damage to attackable contents of a target tile.
   *
   * Applies damage to both a building (if one exists) and a unit 
   * located on the target tile.
   *
   * @param tile The target tile whose contents are attacked.
   * @param damage The amount of damage to deal.
   */
  void dealDamageToTileContents(std::shared_ptr<world::Tile> tile, int damage);

protected:
  /**
   * @brief Protected constructor used internally by the Create() factory method.
   *
   * Initializes core state such as HP, tile, and owner. Does not perform any 
   * registration or map placement logic.
   *
   * @param tile The tile the unit will occupy.
   * @param owner The player that owns the unit.
   * @param max_hp The maximum HP value for this unit.
   * @param build_type The type of this unit.
   */
  Unit(std::shared_ptr<world::Tile> tile,
       std::shared_ptr<core::Player> owner,
       int max_hp,
       UnitType build_type);

  int max_hp_{0};  ///< The maximum health points of the unit.
  int current_hp_{0};  ///< The current health points of the unit.
  bool has_attacked_{false};
  int turn_movement_{0};
  UnitType unit_type_{UnitType::kSoldier};  ///< The type of this unit.

  std::weak_ptr<world::Tile> current_tile_;  ///< Weak reference to the tile this unit occupies.
  std::weak_ptr<core::Player> owner_;        ///< Weak reference to the player that owns this unit.
};


/**
 * @class Soldier
 * @brief A simple combat unit capable of attacking and dealing damage to enemies.
 *
 * Soldiers represent the most basic unit type in the game.
 * They can attack buildings or other units on adjacent tiles.
 */
class Soldier : public Unit {
public:
  /**
   * @brief Creates and registers a Soldier on a tile for a player.
   * @param tile The tile to place the soldier on.
   * @param owner The owning player.
   * @param max_hp The maximum health of the soldier.
   * @return A shared pointer to the created Soldier.
   */
  static std::shared_ptr<Soldier> Create(std::shared_ptr<world::Tile> tile,
                                         std::shared_ptr<core::Player> owner,
                                         int max_hp);

protected:
  Soldier(std::shared_ptr<world::Tile> tile,
          std::shared_ptr<core::Player> owner,
          int max_hp);
};

} // namespace units
