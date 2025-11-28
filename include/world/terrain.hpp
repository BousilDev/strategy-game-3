#pragma once
#include "string"
#include <vector>
#include <list>
#include "core/resource.hpp"

namespace world {

/**
 * @class Terrain
 * @brief Base class representing a terrain type in the game.
 */
class Terrain {
public:
    /**
     * @brief Constructs a Terrain from a std::list of resources.
     * @param name Name of the terrain type.
     * @param movement_cost Movement cost associated with this terrain.
     * @param sight_cost Sight cost associated with this terrain.
     * @param resources List of resources present on the terrain.
     */
    Terrain(const std::string& name, int movement_cost, int sight_cost,
            const std::list<core::Resource>& resources)
        : terrain_name_(name), movement_cost_(movement_cost),
          sight_cost_(sight_cost), resources_(resources) {}

    /**
     * @brief Constructs a Terrain from a std::vector of resources.
     * @param name Name of the terrain type.
     * @param movement_cost Movement cost associated with this terrain.
     * @param sight_cost Sight cost associated with this terrain.
     * @param resources Vector of resources present on the terrain.
     */
    Terrain(const std::string& name, int movement_cost, int sight_cost,
            const std::vector<core::Resource>& resources)
        : terrain_name_(name), movement_cost_(movement_cost),
          sight_cost_(sight_cost), resources_(resources.begin(), resources.end()) {}

    /** @brief Default virtual destructor. */
    virtual ~Terrain() = default;

    /** @brief Returns the terrain name. */
    virtual const std::string& get_name() const { return terrain_name_; }

    /** @brief Returns the movement cost of the terrain. */
    virtual int get_movement_cost() const { return movement_cost_; }

    /** @brief Returns the sight cost of the terrain. */
    virtual int get_sight_cost() const { return sight_cost_; }

    /** @brief Returns a reference to the list of resources on this terrain. */
    virtual std::list<core::Resource>& get_resources() { return resources_; }

    /** @brief Removes tesources from this terrain and passes a list to the deleted resources */
    virtual std::list<core::Resource> take_resources() {
        std::list<core::Resource> copy = std::move(resources_);
        return copy;
    }

protected:
    std::string terrain_name_;///< Name of the terrain type
    int movement_cost_;///< Cost to move across the terrain
    int sight_cost_;///< Cost to see through the terrain
    std::list<core::Resource> resources_;///< List of resources on the terrain
};

class PlainsTerrain : public Terrain {
public:
    // Default resources for Plains
    static std::list<core::Resource> default_resources() {
        return {
            {core::kFood, 2},
            {core::kWood, 2}
        };
    }
    PlainsTerrain(const std::list<core::Resource>& resources = default_resources())
        : Terrain("plains", 1, 1, resources) {}
    PlainsTerrain(const std::vector<core::Resource>& resources)
        : Terrain("plains", 1, 1, resources) {}
};

class ForestTerrain : public Terrain {
public:
    static std::list<core::Resource> default_resources() {
        return {
            {core::kWood, 5},
            {core::kFood, 5}
        };
    }
    ForestTerrain(const std::list<core::Resource>& resources = default_resources())
        : Terrain("forest", 1, 2, resources) {}
    ForestTerrain(const std::vector<core::Resource>& resources)
        : Terrain("forest", 1, 2, resources) {}
};

class MountainsTerrain : public Terrain {
public:
    static std::list<core::Resource> default_resources() {
        return {
            {core::kMetal, 5},
            {core::kGold, 5}
        };
    }
    MountainsTerrain(const std::list<core::Resource>& resources = default_resources())
        : Terrain("mountains", 2, 2, resources) {}
    MountainsTerrain(const std::vector<core::Resource>& resources)
        : Terrain("mountains", 2, 2, resources) {}
};

class WaterTerrain : public Terrain {
public:
    static std::list<core::Resource> default_resources() {
        return {
            //nothing
        };
    }
    WaterTerrain(const std::list<core::Resource>& resources = default_resources())
        : Terrain("water", -1, 1, resources) {}
    WaterTerrain(const std::vector<core::Resource>& resources)
        : Terrain("water", -1, 1, resources) {}
};

} // namespace world
