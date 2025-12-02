#include "world/tile.hpp"
#include "buildings/building.hpp"
#include "core/player.hpp"

namespace world {

std::shared_ptr<Terrain>& Tile::get_terrain(){
    return terrain_;
}
std::shared_ptr<buildings::Building> Tile::get_building(){
    return current_building_;
}
std::array<std::weak_ptr<Tile>, 6>& Tile::get_neighbours(){
    return neighbours_;
}
// TODO: set this tile as the tile of the building.
bool Tile::place_building(std::shared_ptr<buildings::Building> building) {
    if(current_building_){
        return false;
    } else {
    current_building_ = building;
    return true;
    }
}
void Tile::destroy_current_building(){
    current_building_ = nullptr;
}
// TODO: set this tile as the tile of the unit.
bool Tile::place_unit(std::shared_ptr<units::Unit> unit){
    if(current_unit_){
        return false;
    } else {
    current_unit_ = unit;
    return true;
    }
}
void Tile::remove_current_unit(){
    current_unit_=nullptr;
}
void Tile::set_terrain(std::shared_ptr<Terrain> terrain) {
    terrain_ = std::move(terrain);
}

std::vector<unsigned int>& Tile::get_tiles_in_n_range(unsigned int from_this_distance) {
    static std::vector<unsigned int> result;
    result.clear();

    std::queue<std::pair<std::shared_ptr<Tile>, unsigned int>> to_visit;
    std::unordered_set<unsigned int> visited;

    // Start with this tile
    to_visit.push({shared_from_this(), 0}); // Tile must be used with shared_ptr
    visited.insert(tile_number_);

    while (!to_visit.empty()) {
        auto [current_tile, distance] = to_visit.front();
        to_visit.pop();

        // Add to result
        result.push_back(current_tile->get_tile_number());

        if (distance >= from_this_distance) continue;

        // Visit neighbors
        for (auto& weak_neigh : current_tile->get_neighbours()) {
            if (auto neigh = weak_neigh.lock()) {
                unsigned int neigh_number = neigh->get_tile_number();
                if (visited.find(neigh_number) == visited.end()) {
                    visited.insert(neigh_number);
                    to_visit.push({neigh, distance + 1});
                }
            }
        }
    }

    return result;
}

} // namespace world