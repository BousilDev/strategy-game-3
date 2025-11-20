#pragma once
#include "string"
#include <vector>
#include <list>
#include "core/resource.hpp"

namespace world {

class Terrain {
public:
    // Construct from std::list
    Terrain(const std::string& name, int movement_cost, int sight_cost,
            const std::list<core::Resource>& resources)
        : terrain_name_(name), movement_cost_(movement_cost),
          sight_cost_(sight_cost), resources_(resources) {}

    // Construct from std::vector
    Terrain(const std::string& name, int movement_cost, int sight_cost,
            const std::vector<core::Resource>& resources)
        : terrain_name_(name), movement_cost_(movement_cost),
          sight_cost_(sight_cost), resources_(resources.begin(), resources.end()) {}

    virtual ~Terrain() = default;

    virtual const std::string& get_name() const { return terrain_name_; }
    virtual int get_movement_cost() const { return movement_cost_; }
    virtual int get_sight_cost() const { return sight_cost_; }
    virtual std::list<core::Resource>& get_resources() { return resources_; }

    virtual std::list<core::Resource> take_resources() {
        std::list<core::Resource> copy = std::move(resources_);
        return copy;
    }

protected:
    std::string terrain_name_;
    int movement_cost_;
    int sight_cost_;
    std::list<core::Resource> resources_;
};

class PlainsTerrain : public Terrain {
public:
    PlainsTerrain(const std::list<core::Resource>& resources = {})
        : Terrain("plains", 1, 1, resources) {}
    PlainsTerrain(const std::vector<core::Resource>& resources)
        : Terrain("plains", 1, 1, resources) {}
};

class ForestTerrain : public Terrain {
public:
    ForestTerrain(const std::list<core::Resource>& resources = {})
        : Terrain("forest", 1, 2, resources) {}
    ForestTerrain(const std::vector<core::Resource>& resources)
        : Terrain("forest", 1, 2, resources) {}
};

class MountainsTerrain : public Terrain {
public:
    MountainsTerrain(const std::list<core::Resource>& resources = {})
        : Terrain("mountains", 2, 2, resources) {}
    MountainsTerrain(const std::vector<core::Resource>& resources)
        : Terrain("mountains", 2, 2, resources) {}
};

class WaterTerrain : public Terrain {
public:
    WaterTerrain(const std::list<core::Resource>& resources = {})
        : Terrain("water", -1, 1, resources) {}
    WaterTerrain(const std::vector<core::Resource>& resources)
        : Terrain("water", -1, 1, resources) {}
};

} // namespace world
