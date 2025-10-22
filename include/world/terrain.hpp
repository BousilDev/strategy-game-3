#pragma once
#include "string"
#include <vector>
#include "core/resource.hpp"
/**
 * @file Terrain.hpp
 * @brief Declares the Terrain class and its derived classes.
 */

namespace world {
  
/**
 * @class Terrain
 * @brief Abstract base class representing a terrain type.
 * 
 * A Terrain contains information such as movement cost, sight cost, and a set
 * of resources it may contain. A Terrain object should always be held by a Tile object.
 */
class Terrain {
public:
    /**
    * @brief Construct for a terrain object.
    * @note you should construct a terrain by using the inhereted classes.
    * @param name The name of the terrain like plains.
    * @param movement_cost the cost to move to this terrain.
    * @param sight_cost the cost to reveal this terrain.
    * @param resources a vector of resources this terrain has.  
    */
    Terrain(const std::string& name, int movement_cost, int sight_cost,
          std::vector<core::Resource> resources)
        : terrain_name_(name), movement_cost_(movement_cost),
          sight_cost_(sight_cost),  resources_(resources){};

    /**
    * @brief default destructor needed for abstract classes.
    */
    virtual ~Terrain() = default;

    /**
    * @brief getter for the name of this terrain.
    * @return returns the name of this terrain as a string.
    */
    virtual const std::string& get_name() const { return terrain_name_; };

    /**
    * @brief getter for movement_cost_
    * @return returns the movement cost of this terrain. negative values mean you cant move here.
    */
    virtual int get_movement_cost() const { return movement_cost_; };

    /**
    * @brief getter for sight_cost_
    * @return returns the sight cost of this terrain.
    */
    virtual int get_sight_cost() const { return sight_cost_; };

    /**
    * @brief getter for resources_.
    * @return returns a reference to a vector containing this terrains resource structs. might be empty if this terrain has no resources.
    */
    virtual std::vector<core::Resource>& get_resources() { return  resources_; };

    /**
    * @brief Retrieves and removes all resources from this terrain.
    * 
    * This method returns a copy of the resources vector and clears the terrain's internal
    * resources, effectively "taking" them.
    * 
    * @return A vector containing all resources that were in this terrain.
    */
    virtual std::vector<core::Resource> take_resources() {
        std::vector<core::Resource> copy = std::move(resources_);
        return copy;
    };
protected:
    std::string terrain_name_;///< the name of this terrain
    int movement_cost_;///< the movement cost to this terrain. Negative means no movement.
    int sight_cost_;///< the cost to reveal this terrain.
    std::vector<core::Resource> resources_;///<resources of this terrain. might be empty.
};


/**
 * @class PlainsTerrain
 * @brief Represents a plains terrain type.
 * 
 */
class PlainsTerrain : public Terrain{
public:
    /**
     * @brief Constructs a PlainsTerrain with optional resources.
     * @param resources Vector of resources contained in the terrain.
     */
    PlainsTerrain(std::vector<core::Resource> resources) 
        : Terrain("plains", 1, 1, resources){};
};

/**
 * @class ForestTerrain
 * @brief Represents a forest terrain type.
 * 
 */
class ForestTerrain : public Terrain{
public:
    /**
     * @brief Constructs a ForestTerrain with optional resources.
     * @param resources Vector of resources contained in the terrain.
     */
    ForestTerrain(std::vector<core::Resource> resources) 
        : Terrain("forest", 1, 2, resources){};
};

/**
 * @class MountainsTerrain
 * @brief Represents a mountains terrain type.
 * 
 */
class MountainsTerrain : public Terrain{
public:
    /**
     * @brief Constructs a MountainsTerrain with optional resources.
     * @param resources Vector of resources contained in the terrain.
     */
    MountainsTerrain(std::vector<core::Resource> resources) 
        : Terrain("mountains", 2, 2, resources){};
};

/**
 * @class WaterTerrain
 * @brief Represents a water terrain type.
 * 
 */
class WaterTerrain : public Terrain{
public:
    /**
     * @brief Constructs a WaterTerrain with optional resources.
     * @param resources Vector of resources contained in the terrain.
     */
    WaterTerrain(std::vector<core::Resource> resources) 
        : Terrain("water", -1, 1, resources){};
};
} // namespace world