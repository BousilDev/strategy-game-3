#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "world/tile.hpp"
#include "world/terrain.hpp"
/**
 * @file Map.hpp
 * @brief Declares the Map class which manages and stores the state of the game world.
 */

namespace world {

/**
 * @class Map
 * @brief Manages a hexagonal tile map (see diagram below).
 *
 * The tiles are their own class. The map is n × m in size, and all tiles
 * are in one vector of length n*m. The top-left tile is 0; tiles increase
 * left-to-right, top-to-bottom.
 * When map object is created with non-empty constructor the map is generated 
 * using default generator and is ready for use.
 *
 * Example layout:
 * @verbatim
 *  / \ / \ / \
 * | 0 | 1 | 2 |
 *  \ / \ / \ /\
 *   | 3 | 4 | 5|
 *  / \ / \ / \ /
 * | 6 | 7 | 8 |
 *  \ / \ / \ /
 * @endverbatim
 */
class Map {
public:
    /**
    * @brief default constructor constructs a map object but doesnt set tiles.
    * @deprecated Dont use default constructor.
    */
    Map();

    /**
    * @brief Construct a Map with given dimensions.
    * @param map_width The number of columns.
    * @param map_height The number of rows.
    */
    Map(unsigned int map_width, unsigned int map_length);

    /**
    * @brief Construct a Map with the given integer as the number of rows AND columns.
    * @param map_size defines both the number of rows and the number of columns.
    */
    Map(unsigned int map_size);

     /**
    * @brief A generator that is called when a map is constructed. Fills the tiles_ vector with tiles
    * and the tiles with the right terrain and resources.
    * @note This is currently very simple constructor that defines all tiles with no resources
    * and as plain terrain. Create multiple different generators which can be chosen from.
    */
    void generate_map();

    /** @brief For getting tiles from the tile_ vector. Since the tiles are in a 1D vector calling
    * for a tile <1, 3> is called get_tile(3*map_width + 1) 
    * @param tile_number n:th tile in tile vector
    * @return returns a new shared pointer that contains the decired tile. If the tile number is
    * out of range returns empty shared pointer
    */
    std::shared_ptr<Tile> get_tile(unsigned int tile_number);

    /** @brief getter for tiles_ vector.
    * @return returns a reference to the tiles_ vector.
    */
    std::vector<std::shared_ptr<Tile>>& get_tiles();

    /** @brief chooses n tiles and returns them in shared pointers in a new vector. 
    * A tile can be chosen as a spwan if it has plains terrain.
    * @param nof_players the number of chosen spawn-tiles returned.
    * @return returns a vector with shared pointers  that contain the tiles. If for some reason,
    * like not enough tiles existing, some pointers might be null.
    */
    std::vector<std::shared_ptr<Tile>> get_n_spawn(unsigned int nof_players);

    /** @brief Prints the map in the console. Each tile is represented by the first letter
    * of their terrain.
    */
    void print_map() const;

    /** @brief getter for map_width_
    * @return returns the number of colums.
    */
    unsigned int get_map_width() const;

    /** @brief getter for map_length_
    * @return returns the number of rows.
    */
    unsigned int get_map_height() const;

    /** @brief temporary test 
    * function made by AI
    */
    void flood_tile_neighbours_test(std::shared_ptr<Tile> tile) {
    if (!tile) return;
    // Loop over all 6 neighbors
    for (auto& weak_neighbor : tile->get_neighbours()) {
        if (auto neighbor = weak_neighbor.lock()) { // check if neighbor exists
            // Create a new WaterTerrain and assign it to neighbor
            auto water = std::make_shared<WaterTerrain>(std::vector<core::Resource>());
            neighbor->set_terrain(water);
        }
    }
};
private:
    std::vector<std::shared_ptr<Tile>> tiles_; ///< 1D vector of tiles the map has.
    unsigned int map_width_; ///< The number of columns of tiles.
    unsigned int map_lenght_; ///< The number of rows of tiles.
};
  
} // namespace world
