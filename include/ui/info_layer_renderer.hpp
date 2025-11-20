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
    InfoLayerRenderer() {};
    void Initialize(core::Game& game, const std::shared_ptr<sf::Font> font);

    void DrawTo(sf::RenderWindow& window);
    void Update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, std::shared_ptr<world::Tile> tile_pointer);

private: 
    core::Game* game_ = nullptr;
    float tile_size_;
    std::shared_ptr<sf::Font> font_;
    std::vector<DrawItem<std::string>> items_;
    std::vector<DrawItem<std::string>> resource_items_;
    std::shared_ptr<world::Tile> selected_tile_ = nullptr;
    std::shared_ptr<cards::Card> selected_card_ = nullptr;

    sf::RectangleShape background_;
    sf::RectangleShape nextTurnButton_;
    sf::Text nextTurnText_;
    sf::RectangleShape tileInfoBackground_;
    sf::Text tileInfoText_;
    sf::RectangleShape infoBackground_;
    sf::Text infoText_;
    sf::RectangleShape cardBackground_;
    std::vector<sf::RectangleShape> cardBackgrounds_;

    int parts_ = 2;
    float inc_ = (constants::infoLayerHeight - float(constants::infoLayerTextSize)) / float(parts_);

    void UpdateDrawItems();
    sf::Vector2f GetFixedPosition(sf::RenderWindow& window, const sf::Vector2f& relativePos, bool absolute = true);
    int DrawItemAtLocation(sf::RenderWindow& window, DrawItem<std::string> item, sf::Vector2f location);
    void DrawItems(sf::RenderWindow& window, std::vector<DrawItem<std::string>> items, int& textLen);
    void DrawCardAtLocation(sf::RenderWindow& window, std::shared_ptr<cards::Card> card, sf::Vector2f location, int index);
    void DrawCards(sf::RenderWindow& window);
    std::string GetTileInfoString();
    std::string GetCardInfoString(std::shared_ptr<cards::Card> card);

    bool isNextTurnClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos);
    std::shared_ptr<cards::Card> CardClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos);
};

} // namespace ui