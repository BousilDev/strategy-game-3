#include "ui/info_layer_renderer.hpp"

void ui::InfoLayerRenderer::UpdateDrawItems() {
    items_.clear();
    items_.push_back({"Turn", std::to_string(game_.GetCurrentTurn())});
    if (constants::debug) {
        items_.push_back({"Players", std::to_string(game_.GetNofPlayers())});
    }
    items_.push_back({"Current Player", game_.GetCurrentPlayer().GetName()});
    std::array<core::Resource, 4UL> resources = game_.GetCurrentPlayer().GetResources();
    for (int i = 0; i < constants::resourceTypeNames.size(); ++i) {
        items_.push_back({constants::resourceTypeNames[i], std::to_string(resources[i].amount)});
    }
    items_.push_back({"Buildings", std::to_string(game_.GetCurrentPlayer().GetBuildings().size())});
    items_.push_back({"Units", std::to_string(game_.GetCurrentPlayer().GetUnits().size())});
    // Add more game state info as needed
}

sf::Vector2f ui::InfoLayerRenderer::GetFixedPosition(sf::RenderWindow& window, const sf::Vector2f& relativePos) {
    sf::Vector2f viewSize = window.getView().getSize();
    float origX = constants::kInitWindowWidth;
    float origY = constants::kInitWindowHeight;
    float scaleX = viewSize.x / origX;
    float scaleY = viewSize.y / origY;
    sf::Vector2f scaledPos(relativePos.x * scaleX, relativePos.y * scaleY);
    sf::Vector2f diff = window.getView().getCenter() - viewSize / 2.f;
    return diff + scaledPos;
}

void ui::InfoLayerRenderer::DrawTo(sf::RenderWindow& window) {
    UpdateDrawItems();
    sf::RectangleShape background;
    sf::Vector2f winSize = window.getView().getSize();
    background.setSize(sf::Vector2f(winSize.x, constants::infoLayerHeight));
    background.setFillColor(sf::Color(25, 217, 255, 255));

    // Keep background fixed relative to the view
    sf::Vector2f diff = GetFixedPosition(window, sf::Vector2f(0.f, 0.f));
    background.setPosition(diff);
    window.draw(background);

    for (const auto& item : items_) {
        sf::Text text;
        text.setFont(*font_);
        text.setString(item.description + ": " + item.value);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        text.setPosition(10.f, 10.f + (&item - &items_[0]) * 25.f);
        window.draw(text);
    }
}