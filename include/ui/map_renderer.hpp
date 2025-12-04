#pragma once
#include "constants/constants.hpp"
#include <SFML/Graphics.hpp>
#include "world/map.hpp"
#include "core/game.hpp"
#include <set>
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

    void UpdateVisibleTiles();

    void Update(sf::RenderWindow& window, const sf::Event& event);

   private:

    // SFML shapes 
    float tile_size_;
    std::vector<sf::CircleShape> tiles_;
    std::vector<sf::RectangleShape> buildings_;

    // Game info
    std::shared_ptr<world::Tile> selected_tile_ = nullptr;
    std::set<unsigned int> visible_tiles_;
    core::Game* game_ = nullptr;
    std::vector<std::shared_ptr<world::Tile>> spawn_tiles_;
    int last_turn_;
    world::Map map_;
    
};

} // namespace ui
