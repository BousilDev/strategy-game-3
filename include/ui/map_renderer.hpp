#pragma once
#include <SFML/Graphics.hpp>
#include "world/map.hpp"
#define TILE_SIZE 32.f

class MapRenderer {
public:
    MapRenderer(world::Map map, sf::RenderWindow& window, float tile_size = TILE_SIZE);

    void DrawTo();

private:
    world::Map map_;
    float tile_size_;
    std::vector<sf::CircleShape> tiles_;
    sf::RenderWindow& window_;

    void BuildTiles();
    sf::Vector2f hex_to_pixel(unsigned int index) const;
};
