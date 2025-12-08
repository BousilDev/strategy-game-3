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

        int width = static_cast<int>(map_width);
        int length = static_cast<int>(map_length);

        for(int j=0; j < length; j++){
            for(int i=0; i < width; i++){
                // relative positions for a row.
                int right = i + 1;
                int left = i - 1;
                int tb_right = i + (j % 2);
                int tb_left = i - ((j+1) % 2);
                // check validity of position.
                if(right < width) {
                    tiles[j*width + i]->get_neighbours()[0]
                    = tiles[j*width + right];
                }
                if(left >= 0) {
                    tiles[j*width + i]->get_neighbours()[3]
                    = tiles[j*width + left];
                }
                if(tb_right < width) {
                    if(j - 1 >= 0){
                        tiles[j*width + i]->get_neighbours()[1]
                        = tiles[(j-1)*width + tb_right];
                    }
                    if(j + 1 < length){
                        tiles[j*width + i]->get_neighbours()[5]
                        = tiles[(j+1)*width + tb_right];
                    }
                }
                if(tb_left >= 0){
                    if(j - 1 >= 0){
                        tiles[j*width + i]->get_neighbours()[2]
                        = tiles[(j-1)*width + tb_left];
                    }
                    if(j + 1 < length){
                        tiles[j*width + i]->get_neighbours()[4]
                        = tiles[(j+1)*width + tb_left];
                    }
                }
        }
    }
}

Map::Map(unsigned int map_width, unsigned int map_length, GenerationMethod generationmethod) 
 : map_height_(map_length), map_width_(map_width ){
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
    const int total_tiles = map_width_ * map_height_;
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
case GenerationMethod::BigDroplets: {
    const int total_tiles = map_width_ * map_height_;
    const double target_fraction = 0.10; // fraction of map per terrain type
    const int min_radius = 1;
    const int max_radius = 3;

    // Fill map with plains first
    for (auto& tile : tiles_) {
        tile->set_terrain(std::make_shared<PlainsTerrain>());
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> tile_dist(0, total_tiles - 1);
    std::uniform_int_distribution<int> radius_dist(min_radius, max_radius);

    auto apply_droplet = [&](size_t center_idx, auto terrain_factory, int radius) {
        auto& center_tile = tiles_[center_idx];
        std::vector<unsigned int> neighbour_indices = center_tile->get_tiles_in_n_range(radius);

        for (auto idx : neighbour_indices) {
            tiles_[idx]->set_terrain(terrain_factory());
        }
    };

    auto drop_terrain = [&](auto terrain_factory) {
        int tiles_to_cover = static_cast<int>(total_tiles * target_fraction);
        int covered = 0;

        while (covered < tiles_to_cover) {
            size_t center_idx = tile_dist(rng);
            int radius = radius_dist(rng);
            apply_droplet(center_idx, terrain_factory, radius);

            covered += radius * radius; // approximate coverage
        }
    };

    // Spawn terrains
    drop_terrain([&]() { return std::make_shared<WaterTerrain>(); });
    drop_terrain([&]() { return std::make_shared<ForestTerrain>(); });
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
    for (size_t j = 0; j < map_height_; ++j) {
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
    return map_height_;
};


std::vector<std::shared_ptr<Tile>> Map::get_n_spawn(unsigned int nof_players)
{
    std::vector<size_t> plains_indices;
    for (size_t i = 0; i < tiles_.size(); ++i) {
        if (tiles_[i]->get_terrain()->get_name() == "plains") {
            plains_indices.push_back(i);
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd());

    int min_distance = static_cast<int>(map_width_ * 0.20f + 1);
    if (min_distance < 1) min_distance = 1;
    std::cout << "min distance: " << min_distance;

    // ---------- Try spaced placement ----------
    static constexpr int MAX_ATTEMPTS = 10;

    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        std::shuffle(plains_indices.begin(), plains_indices.end(), rng);

        std::vector<size_t> chosen;

        for (size_t idx : plains_indices) {
            bool too_close = false;
            for (size_t prev : chosen) {
                if (distance(static_cast<int>(idx),
                             static_cast<int>(prev)) < min_distance) {
                    too_close = true;
                    break;
                }
            }

            if (!too_close) {
                chosen.push_back(idx);
                if (chosen.size() == nof_players)
                    break;
            }
        }

        if (chosen.size() == nof_players) {
            std::vector<std::shared_ptr<Tile>> result;
            for (size_t idx : chosen)
                result.push_back(tiles_[idx]);
            return result;
        }
    }

    // ---------- Fallback: ignore spacing ----------
    std::cout << "[Map::get_n_spawn] Using fallback (ignoring spacing rule)\n";

    std::shuffle(plains_indices.begin(), plains_indices.end(), rng);

    std::vector<std::shared_ptr<Tile>> result;
    for (size_t i = 0;
         i < std::min(plains_indices.size(),
                      static_cast<size_t>(nof_players));
         ++i)
    {
        result.push_back(tiles_[plains_indices[i]]);
    }

    return result;
}


//uses coordinate conversion to calculate distance
int Map::distance(int tile1, int tile2) const {
        // 1D index → row/col
        int col1 = tile1 % map_width_;
        int row1 = tile1 / map_width_;
        int col2 = tile2 % map_width_;
        int row2 = tile2 / map_width_;

        // Even-r layout conversion (second row shifted right)
        int q1 = col1 - (row1 / 2);
        int r1 = row1;
        int q2 = col2 - (row2 / 2);
        int r2 = row2;

        // Axial → cube coordinates
        int x1 = q1, z1 = r1, y1 = -x1 - z1;
        int x2 = q2, z2 = r2, y2 = -x2 - z2;

        // Cube distance
        return std::max({abs(x1 - x2), abs(y1 - y2), abs(z1 - z2)});
}


std::ostream& operator<<(std::ostream& out, const Map& other){
    out << other.get_map_width() << "\n";
    out << other.get_map_height() << "\n";
    for (const auto& tile_ptr : other.get_tiles()) {
        out << static_cast<int>(tile_ptr->get_terrain()->get_terrain_type()) << ",";
    }
    out << "\n";
    return out;
};
std::istream& operator>>(std::istream& in, Map& other){
    int width = core::GetIntFromLine(in);
    int height = core::GetIntFromLine(in);
    std::vector<int> terrain_types_vector = core::GetIntVectorFromLine(in);

    other = Map(width, height); // construct new map directly

    auto& tiles = other.get_tiles(); // if this returns a non-const reference

    for (size_t i = 0; i < terrain_types_vector.size() && i < tiles.size(); i++) {
        std::shared_ptr<Terrain> terrain_ptr;
        switch (terrain_types_vector[i] % 4) {
            case 0: terrain_ptr = std::make_shared<PlainsTerrain>(); break;
            case 1: terrain_ptr = std::make_shared<ForestTerrain>(); break;
            case 2: terrain_ptr = std::make_shared<MountainsTerrain>(); break;
            case 3: terrain_ptr = std::make_shared<WaterTerrain>(); break;
        }
        tiles[i]->set_terrain(terrain_ptr);
    }

    return in;
}
} // namespace world