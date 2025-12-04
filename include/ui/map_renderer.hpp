#pragma once
#include "constants/constants.hpp"
#include <SFML/Graphics.hpp>
#include "world/map.hpp"
#include "core/game.hpp"
#define TILE_SIZE 64.f

namespace ui {

class MapRenderer {
public:
    MapRenderer() {}
    void Initialize(core::Game& game, sf::RenderWindow& window, float tile_size = TILE_SIZE);

    void DrawTo(sf::RenderWindow& window);

    void ClickTile(sf::RenderWindow& window);

    std::shared_ptr<world::Tile> GetClickedTile(sf::RenderWindow& window);

    std::shared_ptr<world::Tile> GetLastClickedTile();

    void UpdateOutsideEventLoop(sf::RenderWindow& window);

    void PanMap(sf::RenderWindow& window);

    void SetViewOnPlayer(sf::RenderWindow& window);

   private:
    core::Game* game_ = nullptr;
    std::vector<std::shared_ptr<world::Tile>> spawn_tiles_;
    int last_turn_;
    world::Map map_;
    float tile_size_;
    std::vector<sf::CircleShape> tiles_;
    std::vector<sf::RectangleShape> buildings_;
    std::shared_ptr<world::Tile> selected_tile_ = nullptr;
    void BuildTiles();
    sf::Vector2f hex_to_pixel(unsigned int index) const;
};

} // namespace ui
