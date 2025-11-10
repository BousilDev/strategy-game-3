#include "ui/info_layer_renderer.hpp"

void ui::InfoLayerRenderer::UpdateDrawItems() {
    items_.clear();
    items_.push_back({"Turn", std::to_string(game_.GetCurrentTurn())});
    items_.push_back({"Players", std::to_string(game_.GetNofPlayers())});
    items_.push_back({"Current Player", game_.GetCurrentPlayer().GetName()});
    items_.push_back({"Resources", std::to_string(game_.GetCurrentPlayer().GetResources()[0].amount)});
    // Add more game state info as needed
}

void ui::InfoLayerRenderer::DrawTo(sf::RenderWindow& window) {
    UpdateDrawItems();
    for (const auto& item : items_) {
        sf::Text text;
        text.setFont(*font_);
        text.setString(item.description + ": " + item.value);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        text.setPosition(10.f, 10.f + (&item - &items_[0]) * 25.f);
        window.draw(text);
    }
}