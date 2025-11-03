#pragma once
#include <SFML/Graphics.hpp>
#include "world/map.hpp"

class MapRenderer {
public:
    MapRenderer(world::Map map, float tile_size = 32.f);

    void DrawTo(sf::RenderWindow& window);

private:
    world::Map map_;
    float tile_size_;
    std::vector<sf::CircleShape> tiles_;

    void BuildTiles();
    sf::Vector2f hex_to_pixel(unsigned int index) const;
};
