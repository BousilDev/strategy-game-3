#include "ui/map_renderer.hpp"
#include "ui/center_origin.hpp"
#include <cmath>

// #define HEX_ROTATION 0.f

MapRenderer::MapRenderer(world::Map map,
    float tile_size) : map_(map), tile_size_(tile_size) {

    const size_t width = map_.get_map_width();
    //const size_t height = map_.get_map_height();
    
    
    const auto & tile_data = map_.get_tiles();

    const float h  = std::sqrt(3.f) * tile_size_; // horizontal spacing
    const float v  = 1.5f * tile_size_;           // vertical spacing
    
    for (size_t i = 0; i < tile_data.size(); i++) {
        size_t row = i / width;
        size_t col = i % width;
        
        sf::Vector2f pos = sf::Vector2f(col * h + ((row & 1) ? h * 0.5 : 0.0)+400,
                                        row * v+400
        );
        

        sf::CircleShape tile(tile_size_, 6);
        ui::centerOrigin(tile);
//        tile.setRotation(HEX_ROTATION);
        tile.setPosition(pos);

/*         float map_width_px = (width - 1) * h + h;
        float map_height_px = ((tile_data.size() / width) - 1) * v + v; */

        
        // Switch statement with terrain type instead?
        std::string terrain = tile_data[i]->get_terrain()->get_name();

        if (terrain == "plains")
            tile.setFillColor(sf::Color(132,176,103));
        else if (terrain == "forest")
            tile.setFillColor(sf::Color(93,101,50));
        else if (terrain == "mountains")
            tile.setFillColor(sf::Color(203,203,203));
        else if (terrain == "water")
            tile.setFillColor(sf::Color(0,123,167));
        else
            tile.setFillColor(sf::Color::White);

        tile.setOutlineColor(sf::Color(0,0,0));
        tile.setOutlineThickness(2);
        tiles_.push_back(tile);
            }
}

void MapRenderer::DrawTo(sf::RenderWindow& window)  {
    
    for (auto v :  tiles_) {
        window.draw(v);
    }
};


