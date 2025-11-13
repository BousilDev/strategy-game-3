#pragma once

#include "world/tile.hpp"
#include "core/player.hpp"
#include <memory>

/**
 * @file building.hpp
 * @brief Declares the Building class hierarchy.
 * 
 * Buildings are static structures placed on the game map. 
 * Each building belongs to a Player and is placed on a Tile.
 * They can provide various effects or resources during the game, 
 * particularly through the `atTurnEnd()` function.
 */

namespace buildings {

/**
 * @enum BuildingType
 * @brief Enumerates all possible building types in the game.
 */
enum class BuildingType {
  kCapital,       ///< A player's central base structure.
  kFarm,          ///< Produces resources each turn.
  kMine,          ///< Extracts minerals or valuable resources.
  kNeighbourhood, ///< May increase population or provide bonuses.
  kPower          ///< Provides energy or boosts production.
};

/**
 * @class Building
 * @brief Base class representing a structure placed on a map tile.
 *
 * The `Building` class models a fixed structure that belongs to a player 
 * and is placed on a tile. It defines shared functionality for all building types, 
 * such as hit points, ownership, and registration logic.
 * 
 * Ownership model:
 * - Tiles and Players hold **shared ownership** of a building.
 * - Buildings hold **weak references** back to their tile and owner 
 *   to avoid circular reference cycles.
 *
 * Derived classes such as `CapitalBuilding` and `FarmBuilding` implement 
 * specialized per-turn effects by overriding `atTurnEnd()`.
 */
class Building : public std::enable_shared_from_this<Building> {
public:
  /**
   * @brief Virtual destructor for safe polymorphic cleanup.
   */
  virtual ~Building() = default;

  /**
   * @brief Factory method to safely construct and register a building.
   *
   * Creates a new building instance wrapped in a `std::shared_ptr`, 
   * registers it with its tile and owner if valid, 
   * and ensures safe shared ownership handling.
   *
   * @param tile The tile where the building will be placed.
   * @param owner The player who owns this building.
   * @param max_hp The maximum health points of the building.
   * @param build_type The type of building (e.g., farm, capital, etc.).
   * @return A `std::shared_ptr` to the newly created building.
   */
  static std::shared_ptr<Building> Create(std::shared_ptr<world::Tile> tile,
                                          std::shared_ptr<core::Player> owner,
                                          int max_hp,
                                          BuildingType build_type);

  /**
   * @brief Gets the type of this building.
   * @return The building type.
   */
  BuildingType GetType() const { return building_type_; }

  /**
   * @brief Gets the maximum HP (health points) of this building.
   * @return The maximum HP value.
   */
  int getMaxHp() const { return max_hp_; }

  /**
   * @brief Gets the current HP (health points) of this building.
   * @return The current HP value.
   */
  int getCurrentHp() const { return current_hp_; }

  /**
   * @brief Applies damage to this building, reducing its HP.
   *
   * The HP will not fall below zero.
   *
   * @param damage The amount of damage to apply.
   * @return The remaining HP after applying damage.
   */
  int takeDamage(int damage);

  /**
   * @brief Performs any end-of-turn effects specific to this building.
   *
   * Default implementation does nothing. 
   * Derived building types override this to implement custom behavior 
   * (e.g., resource generation for farms).
   */
  virtual void atTurnEnd() {}

protected:
  /**
   * @brief Protected constructor used internally by the factory method.
   *
   * Initializes shared attributes like hit points, owner, and tile associations.
   * Does not perform registration or placement — that is handled by `Create()`.
   *
   * @param tile The tile on which this building resides.
   * @param owner The player who owns this building.
   * @param max_hp The maximum hit points.
   * @param build_type The specific type of this building.
   */
  Building(std::shared_ptr<world::Tile> tile,
           std::shared_ptr<core::Player> owner,
           int max_hp,
           BuildingType build_type);

  int max_hp_{0};   ///< The maximum health of this building.
  int current_hp_{0}; ///< The current health of this building.
  BuildingType building_type_{BuildingType::kFarm}; ///< The type of this building.

  std::weak_ptr<world::Tile> current_tile_;   ///< Weak reference to the tile this building occupies.
  std::weak_ptr<core::Player> owner_;         ///< Weak reference to the player that owns this building.
};

/**
 * @class CapitalBuilding
 * @brief A special building that serves as a player's main base or capital.
 *
 * The CapitalBuilding may serve as the player's headquarters, 
 * often required for victory conditions or resource management.
 */
class CapitalBuilding : public Building {
public:
  /**
   * @brief Factory method to create and register a new CapitalBuilding.
   *
   * @param tile The tile to place the capital on.
   * @param owner The owning player.
   * @param max_hp The maximum HP of the capital.
   * @return A shared pointer to the created CapitalBuilding.
   */
  static std::shared_ptr<CapitalBuilding> Create(std::shared_ptr<world::Tile> tile,
                                                 std::shared_ptr<core::Player> owner,
                                                 int max_hp);

  /**
   * @brief Executes per-turn effects for the capital.
   *
   * Default implementation currently performs no actions.
   */
  void atTurnEnd() override;

protected:
  /**
   * @brief Constructs a CapitalBuilding.
   *
   * This constructor is protected; use `Create()` to instantiate safely.
   *
   * @param tile The tile where the building is located.
   * @param owner The owning player.
   * @param max_hp The maximum HP of the building.
   */
  CapitalBuilding(std::shared_ptr<world::Tile> tile,
                  std::shared_ptr<core::Player> owner,
                  int max_hp);
};

/**
 * @class FarmBuilding
 * @brief Represents a farm structure that produces resources each turn.
 *
 * Farms generate resources based on the terrain they are placed on.
 */
class FarmBuilding : public Building {
public:
  /**
   * @brief Factory method to create and register a new FarmBuilding.
   *
   * @param tile The tile to place the farm on.
   * @param owner The owning player.
   * @param max_hp The maximum HP of the farm.
   * @return A shared pointer to the created FarmBuilding.
   */
  static std::shared_ptr<FarmBuilding> Create(std::shared_ptr<world::Tile> tile,
                                              std::shared_ptr<core::Player> owner,
                                              int max_hp);

  /**
   * @brief Executes per-turn effects for the farm.
   *
   * Typically, this adds terrain-specific resources to the owning player.
   */
  void atTurnEnd() override;

protected:
  /**
   * @brief Constructs a FarmBuilding.
   *
   * This constructor is protected; use `Create()` to instantiate safely.
   *
   * @param tile The tile where the building is located.
   * @param owner The owning player.
   * @param max_hp The maximum HP of the building.
   */
  FarmBuilding(std::shared_ptr<world::Tile> tile,
               std::shared_ptr<core::Player> owner,
               int max_hp);
};

} // namespace buildings
