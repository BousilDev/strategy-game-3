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
bool Tile::place_building(buildings::Building& building){
    if(current_building_){
        return false;
    } else {
    current_building_ =  std::make_shared<buildings::Building>(building);
    return true;
    }
}
void Tile::destroy_current_building(){
    current_building_ = nullptr;
}
bool Tile::place_unit(units::Unit& unit){
    if(current_unit_){
        return false;
    } else {
    current_unit_ =  std::make_shared<units::Unit>(unit);
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