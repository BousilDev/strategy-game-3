#pragma once
#include "constants/constants.hpp"
#include <SFML/Graphics.hpp>
#include "world/map.hpp"
#define TILE_SIZE 48.f

namespace ui {

class MapRenderer {
public:
    MapRenderer() {}
    void Initialize(world::Map map, sf::RenderWindow& window, float tile_size = TILE_SIZE);

    void DrawTo(sf::RenderWindow& window);

    void ClickTile(sf::RenderWindow& window);

    std::shared_ptr<world::Tile> GetClickedTile(sf::RenderWindow& window);

    std::shared_ptr<world::Tile> GetLastClickedTile(sf::RenderWindow& window);

   private:
    world::Map map_;
    float tile_size_;
    std::vector<sf::CircleShape> tiles_;
    std::shared_ptr<world::Tile> selected_tile_ = nullptr;
    //sf::RenderWindow& window_;

    void BuildTiles();
    sf::Vector2f hex_to_pixel(unsigned int index) const;
};

} // namespace ui
