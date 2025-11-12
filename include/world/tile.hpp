#pragma once

/**
 * @file tile.hpp
 * @brief Declares the Tile class which is a single position on the game map. Has a Terrain 
 * and can contain a building, a unit and multiple effects.
 */
#include <vector>
#include <memory>
#include <array>
#include "terrain.hpp"
#include "buildings/building.hpp"
#include "units/unit.hpp"
namespace world {

/**
 * @class Map
 * @brief Tile class deals with tiles that exist on a map.
 *
 * Each tile that is generated when a map is created also contains a terrain. 
 * Tiles possible contain a building, unit and effects.
 * 
 * Tiles know their neighbours. But not their general location on a map. 
 */
class Tile {
    public:
        /**
        * @brief default constructor that constructs an empty tile. A tile's contents like terrain should be set seperately. 
        */
        Tile() = default;

        /**
        * @brief getter for the terrain of this Tile.
        * @return returns a reference to terrain_ that has a sharedpointer holding this tiles terrain.   
        */
        std::shared_ptr<Terrain>& get_terrain();

        /**
        * @brief Setter for setting this tiles terrain.
        * @param terrain shared pointer to a terrain object that this tile should be made to contain.  
        */
        void set_terrain(std::shared_ptr<Terrain> terrain);

    
        /**
        * @brief getter for the building this tile contains.
        * @return returns a copy of current_building_ shared pointer. This might be empty.
        */
        std::shared_ptr<buildings::Building> get_building();

//std::list<Effect>& get_effect();

        /**
        * @brief Setter for the building this tile is made to contain. Does't work if this tile already 
        * contains a building.
        * @param building the building this tile is made to contain.
        * @return returns true if the setting was successful and false if it wasn't.
        */
        bool place_building(std::shared_ptr<buildings::Building> building);

        /**
        * @brief Makes the current_builder_ shared pointer empty. 
        * This function will work even if this tile contains no building.
        */
        void destroy_current_building();

        /**
        * @brief Setter for the unit this tile is made to contain. Does't work if this tile already 
        * contains a unit.
        * @param unit the unit this tile is made to contain.
        * @return returns true if the setting was successful and false if it wasn't.
        */
        bool place_unit(std::shared_ptr<units::Unit> unit);

        /**
        * @brief Makes the current_unit_ shared pointer empty. 
        * This function will work even if this tile contains no unit
        */
        void remove_current_unit();

        /**
        * @brief Getter for neighbours_.
        * @return Returns a reference to an Array of weak pointers that contain a Tile or are null.
        * The size of the array 6 since the tile has max 6 neighbours.
        * The order in the returned array is starting from rightmost neighbour counter-clockwise. nullptr means no neighbour. 
        * Since the array has weak pointers that might be null must do if(auto s_pointer = weak_pointer.lock()) to access. 
        */
        std::array<std::weak_ptr<Tile>, 6>& get_neighbours();

        void set_tile_number(unsigned int num){
            tile_number_ = num;
        }
        unsigned int get_tile_number(){
            return tile_number_;
        }
//void add_effect(Effect);
//void remove_effect(Effect);
        
    protected:
        unsigned int tile_number_;///< the tile number in the tiles_ vector of map.
        std::shared_ptr<Terrain> terrain_;///< The terrain this tile has. also contains resources.
        std::shared_ptr<buildings::Building> current_building_;///< the building this tile might have.
        std::shared_ptr<units::Unit> current_unit_;///< the unit this tile might have.
 //Effect std::list<Effect> effects_;
        std::array<std::weak_ptr<Tile>, 6> neighbours_;///< The neighbours of the tile. The order is from the righmost counter-clockwise. nullptr means no neighbour. 
};
  
} // namespace world
