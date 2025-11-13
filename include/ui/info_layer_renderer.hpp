#pragma once

#include "core/game.hpp"
#include <SFML/Graphics.hpp>

namespace ui {
 
template <typename T>
struct DrawItem {
    std::string description;
    T value;
};

class InfoLayerRenderer {
public:
    InfoLayerRenderer(core::Game& game, const std::shared_ptr<sf::Font> font);

    void DrawTo(sf::RenderWindow& window);
    void Update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event);
    bool isNextTurnClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos);

private: 
    core::Game& game_;
    float tile_size_;
    std::shared_ptr<sf::Font> font_;
    std::vector<DrawItem<std::string>> items_;
    std::vector<DrawItem<std::string>> resource_items_;
    world::Tile* selected_tile_ = nullptr;
    void UpdateDrawItems();
    sf::RectangleShape nextTurnButton_;
    sf::Vector2f GetFixedPosition(sf::RenderWindow& window, const sf::Vector2f& relativePos, bool absolute = true);
    int DrawItemAtLocation(sf::RenderWindow& window, DrawItem<std::string> item, sf::Vector2f location);
};

} // namespace ui