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

Map::Map(unsigned int map_width, unsigned int map_length, GenerationMethod generationmethod) 
 : map_lenght_(map_length), map_width_(map_width ){
    size_t n = map_width * map_length;
    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<Tile> tile = std::make_shared<Tile>();
        tile->set_tile_number(i);
        tiles_.push_back(tile);
    }
    assign_neighbours(map_width, map_length, tiles_);
    generate_map(generationmethod);
    //print_map();
}
Map::Map(unsigned int map_size, GenerationMethod generationmethod)
    : Map(map_size, map_size, generationmethod) {
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
void Map::generate_map(GenerationMethod generationmethod) {
    switch (generationmethod)
    {
    case GenerationMethod::PlainsOnly: {
        for (auto& tile : tiles_) {
            auto plains_ptr = std::make_shared<PlainsTerrain>();
            tile->set_terrain(plains_ptr);
        }
        break;
    }
    case GenerationMethod::Stripes: {
        int row = 0;

        for (size_t i = 0; i < tiles_.size(); ++i) {
        // Compute the current row index
        row = static_cast<int>(i / map_width_);

        std::shared_ptr<Terrain> terrain_ptr;

        // Decide terrain type for this row
        switch (row % 4) {    // 4 types: plains, mountains, forest, water
            case 0:
                terrain_ptr = std::make_shared<PlainsTerrain>();
                break;
            case 1:
                terrain_ptr = std::make_shared<MountainsTerrain>();
                break;
            case 2:
                terrain_ptr = std::make_shared<ForestTerrain>();
                break;
            case 3:
                terrain_ptr = std::make_shared<WaterTerrain>();
                break;
        }

        // Assign a *new* terrain object to each tile
        tiles_[i]->set_terrain(terrain_ptr);
    }

    break;
    }
    case GenerationMethod::Droplets: {
    const int total_tiles = map_width_ * map_lenght_;
    const int tiles_per_droplet = 7; // main + neighbors
    const double target_fraction = 0.10;

    int droplet_count = static_cast<int>((total_tiles * target_fraction) / tiles_per_droplet);

    // Fill with Plains
    for (auto& tile : tiles_) {
        tile->set_terrain(std::make_shared<PlainsTerrain>());
    }

    // Lambda to apply droplet
    auto apply_droplet = [&](int index, auto terrain_factory) {
        tiles_[index]->set_terrain(terrain_factory());
        auto& neigh = tiles_[index]->get_neighbours();
        for (auto& weak : neigh) {
            if (auto nb = weak.lock()) {
                nb->set_terrain(terrain_factory());
            }
        }
    };

    // Random generator
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, total_tiles - 1);

    // Helper to drop N droplets for a terrain type
    auto drop_terrain = [&](auto terrain_factory) {
        for (int i = 0; i < droplet_count; ++i) {
            int idx = dist(rng);
            apply_droplet(idx, terrain_factory);
        }
    };

    // Water droplets
    drop_terrain([&]() { return std::make_shared<WaterTerrain>(); });
    // Forest droplets
    drop_terrain([&]() { return std::make_shared<ForestTerrain>(); });
    // Mountain droplets
    drop_terrain([&]() { return std::make_shared<MountainsTerrain>(); });

    break;
}

    default:
        for (auto& tile : tiles_) {
            auto plains_ptr = std::make_shared<PlainsTerrain>();
            tile->set_terrain(plains_ptr);
        }
        break;
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