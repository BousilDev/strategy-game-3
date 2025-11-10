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
    InfoLayerRenderer(core::Game& game, const std::shared_ptr<sf::Font> font) :
        game_(game), font_(font) {}

    void DrawTo(sf::RenderWindow& window);

private: 
    core::Game& game_;
    float tile_size_;
    std::shared_ptr<sf::Font> font_;
    std::vector<DrawItem<std::string>> items_;
    void UpdateDrawItems();
    sf::Vector2f GetFixedPosition(sf::RenderWindow& window, const sf::Vector2f& relativePos);
};

} // namespace ui