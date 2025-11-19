#include "world/tile.hpp"

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
} // namespace world