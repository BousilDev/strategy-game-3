#include "world/map.hpp"
#include "world/tile.hpp"
#include <memory>
#include <iostream>
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine

namespace world {

//helper function to assign neighbours
void assign_neighbours(unsigned int map_width, unsigned int map_length, 
    std::vector<std::shared_ptr<Tile>>& tiles){
        for(int j=0; j < static_cast<int>(map_length); j++){
            for(int i=0; i < static_cast<int>(map_width); i++){
                // relative positions for a row.
                int right = i + 1;
                int left = i - 1;
                int tb_right = i + (j % 2);
                int tb_left = i - ((j+1) % 2);
                // check validity of position.
                if(right < map_width) {
                    tiles[j*map_width + i]->get_neighbours()[0]
                    = tiles[j*map_width + right];
                }
                if(left >= 0) {
                    tiles[j*map_width + i]->get_neighbours()[3]
                    = tiles[j*map_width + left];
                }
                if(tb_right < map_width) {
                    if(j - 1 >= 0){
                        tiles[j*map_width + i]->get_neighbours()[1]
                        = tiles[(j-1)*map_width + tb_right];
                    }
                    if(j + 1 < map_length){
                        tiles[j*map_width + i]->get_neighbours()[5]
                        = tiles[(j+1)*map_width + tb_right];
                    }
                }
                if(tb_left >= 0){
                    if(j - 1 >= 0){
                        tiles[j*map_width + i]->get_neighbours()[2]
                        = tiles[(j-1)*map_width + tb_left];
                    }
                    if(j + 1 < map_length){
                        tiles[j*map_width + i]->get_neighbours()[4]
                        = tiles[(j+1)*map_width + tb_left];
                    }
                }
        }
    }
}

Map::Map(unsigned int map_width, unsigned int map_length) 
 : map_lenght_(map_length), map_width_(map_width ){
    size_t n = map_width * map_length;
    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<Tile> tile = std::make_shared<Tile>();
        tile->set_tile_number(i);
        tiles_.push_back(tile);
    }
    assign_neighbours(map_width, map_length, tiles_);
    generate_map();
    print_map();
}
Map::Map(unsigned int map_size)
    : Map(map_size, map_size) {
}
Map::Map() {};
std::shared_ptr<Tile> Map::get_tile(unsigned int tile_number){
    if(static_cast<size_t>(tile_number) < tiles_.size()){
        return tiles_[tile_number];
    }
    return nullptr;
}
std::vector<std::shared_ptr<Tile>>& Map::get_tiles(){
    return tiles_;
}
void Map::generate_map() {
    for (auto& tile : tiles_) {
        auto plains_ptr = std::make_shared<PlainsTerrain>(std::vector<core::Resource>());
        tile->set_terrain(plains_ptr);
    }
}
void Map::print_map() const {
    for (size_t j = 0; j < map_lenght_; ++j) {
        if(j % 2 == 1){
            std::cout << " ";
        }
        for (size_t i = 0; i < map_width_; ++i) {
            auto tile = tiles_[j * map_width_ + i];
            if (tile->get_terrain()) {
                std::cout << tile->get_terrain()->get_name()[0] << " "; 
                // print first letter of terrain for compact output
            } else {
                std::cout << "? ";
            }
        }
        std::cout << "\n";
    }
}
unsigned int Map::get_map_width() const{
    return map_width_;
};
unsigned int Map::get_map_height() const{
    return map_lenght_;
};
std::vector<std::shared_ptr<Tile>> Map::get_n_spawn(unsigned int nof_players) {
    std::vector<size_t> valid_tiles;
    for(size_t i = 0; i < tiles_.size(); i++){
        auto tile = tiles_[i];
        if (tile->get_terrain()->get_name() == "plains"){
            valid_tiles.push_back(i);
        }
}

    std::vector<std::shared_ptr<Tile>> result;
    // Shuffle the valid indices for random selection
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(valid_tiles.begin(), valid_tiles.end(), g);

    // Take up to nof_players valid tiles
    size_t count = std::min(valid_tiles.size(), static_cast<size_t>(nof_players));
    for (size_t i = 0; i < count; ++i) {
        size_t index = valid_tiles[i];
        result.push_back(tiles_[index]);
        std::cout << "  -> Assigned tile index " << index 
                  << " (" << tiles_[index]->get_terrain()->get_name() << ")\n";
    }

    // Fill with empty pointers if not enough valid tiles
    while (result.size() < nof_players) {
        result.push_back(nullptr);
        std::cout << "  -> Not enough plains: added empty spawn slot.\n";
    }

    std::cout << "[Map::get_n_spawn] Returning " << result.size() << " spawn tiles.\n";
    return result;
};
} // namespace world