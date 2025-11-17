#include "ui/info_layer_renderer.hpp"
#include <sstream>
#include <string>

void ui::InfoLayerRenderer::UpdateDrawItems() {
    items_.clear();
    resource_items_.clear();
    items_.push_back({"Turn", std::to_string(game_->GetCurrentTurn())});
    if (constants::debug) {
        items_.push_back({"Players", std::to_string(game_->GetNofPlayers())});
    }
    items_.push_back({"Current Player", game_->GetCurrentPlayer().GetName()});
    std::array<core::Resource, 4UL> resources = game_->GetCurrentPlayer().GetResources();
    for (int i = 0; i < constants::resourceTypeNames.size(); ++i) {
        resource_items_.push_back({constants::resourceTypeNames[i], std::to_string(resources[i].amount)});
    }
    items_.push_back({"Buildings", std::to_string(game_->GetCurrentPlayer().GetBuildings().size())});
    items_.push_back({"Units", std::to_string(game_->GetCurrentPlayer().GetUnits().size())});
    // Add more game state info as needed
}

// Returns the fixed position in the window for drawing
sf::Vector2f ui::InfoLayerRenderer::GetFixedPosition(sf::RenderWindow& window, const sf::Vector2f& relativePos, bool absolute) {
    sf::Vector2f viewSize = window.getView().getSize();
    sf::Vector2f diff = window.getView().getCenter() - viewSize / 2.f;
    if (absolute) {
        return diff + relativePos;
    }
    float scaleX = viewSize.x / constants::kInitWindowWidth;
    float scaleY = viewSize.y / constants::kInitWindowHeight;
    sf::Vector2f scaledPos(relativePos.x * scaleX, relativePos.y * scaleY);
    return diff + scaledPos;
}

// Returns the width of the drawn item
int ui::InfoLayerRenderer::DrawItemAtLocation(sf::RenderWindow& window, ui::DrawItem<std::string> item, sf::Vector2f location) {
    sf::Text text;
    text.setFont(*font_);
    text.setString(item.description + ": " + item.value);
    text.setCharacterSize(constants::infoLayerTextSize);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    // Draw background box for the text
    sf::RectangleShape box;
    box.setSize(sf::Vector2f(text.getLocalBounds().width + 10.f, constants::infoLayerTextSize + 5.f));
    box.setFillColor(constants::infoLayerColor + sf::Color(100, 0, 0, 255));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.f);
    box.setPosition(GetFixedPosition(window, location));
    window.draw(box);

    // Set text position to follow the text box position
    text.setPosition(box.getPosition().x + 5.f, box.getPosition().y);
    window.draw(text);
    return std::max(0, static_cast<int>(box.getLocalBounds().width + 10.f));
}

// TODO: Refactor unnecessarily repeated parts to the Initialize method
void ui::InfoLayerRenderer::DrawTo(sf::RenderWindow& window) {
    UpdateDrawItems();
    sf::RectangleShape background;
    sf::Vector2f winSize = window.getView().getSize();
    background.setSize(sf::Vector2f(winSize.x, constants::infoLayerHeight));
    background.setFillColor(constants::infoLayerColor);
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(5.f);

    // Keep background fixed relative to the view
    background.setPosition(GetFixedPosition(window, sf::Vector2f(0.f, 0.f)));
    window.draw(background);

    int textLen = 0;
    int maxTextLen = 0;
    int i = 0;
    int parts = 2;
    float inc = (constants::infoLayerHeight - float(constants::infoLayerTextSize)) / float(parts);
    for (const auto& item : items_) {
        if (i < parts) {
            maxTextLen = std::max(maxTextLen, DrawItemAtLocation(window, item, sf::Vector2f(10.f + textLen, 10.f + i * inc)));
            i++;
        } else {
            textLen += maxTextLen;
            maxTextLen = DrawItemAtLocation(window, item, sf::Vector2f(10.f + textLen, 10.f));
            i = 1;
        }
    }
    // TODO: Draw a separator here between general info and resource info
    i = 0;
    textLen += maxTextLen;
    maxTextLen = 0;
    for (const auto& item : resource_items_) {
        if (i < parts) {
            maxTextLen = std::max(maxTextLen, DrawItemAtLocation(window, item, sf::Vector2f(10.f + textLen, 10.f + i * inc)));
            i++;
        } else {
            textLen += maxTextLen;
            maxTextLen = DrawItemAtLocation(window, item, sf::Vector2f(10.f + textLen, 10.f));
            i = 1;
        }
    }
    textLen += maxTextLen + 10.f;

    // Draw next turn button to top right corner of the info layer
    sf::RectangleShape nextTurnButton;
    nextTurnButton.setSize(sf::Vector2f(120.f, 40.f));
    nextTurnButton.setFillColor(sf::Color(100, 200, 100, 255));
    nextTurnButton.setOutlineColor(sf::Color::White);
    nextTurnButton.setOutlineThickness(2.f);
    nextTurnButton.setPosition(GetFixedPosition(window, sf::Vector2f(10.f + textLen, constants::infoLayerHeight / 2.f - nextTurnButton.getSize().y / 2.f)));
    nextTurnButton_ = nextTurnButton; // Store for click detection
    window.draw(nextTurnButton);

    // Draw next turn text over the button
    sf::Text nextTurnText;
    nextTurnText.setFont(*font_);
    nextTurnText.setString("Next Turn");
    nextTurnText.setCharacterSize(constants::infoLayerTextSize);
    nextTurnText.setFillColor(sf::Color::Black);
    nextTurnText.setStyle(sf::Text::Bold);
    nextTurnText.setPosition(nextTurnButton.getPosition().x + 10.f, nextTurnButton.getPosition().y + 5.f);
    window.draw(nextTurnText);

    // Draw a layer to the right lower corner showing selected tile info
    sf::RectangleShape tileInfoBackground;
    tileInfoBackground.setSize(sf::Vector2f(constants::kInitWindowWidth / 4.f, 2 * constants::kInitWindowHeight / 3.f));
    tileInfoBackground.setFillColor(constants::infoLayerColor);
    tileInfoBackground.setOutlineColor(sf::Color::White);
    tileInfoBackground.setOutlineThickness(5.f);
    tileInfoBackground.setPosition(GetFixedPosition(window, sf::Vector2f(winSize.x - (tileInfoBackground.getSize().x + 10.f), winSize.y - (tileInfoBackground.getSize().y + 10.f))));
    window.draw(tileInfoBackground);

    // Draw tile info
    sf::Text tileInfoText;
    tileInfoText.setFont(*font_);
    std::stringstream ss;
    ss << "Selected Tile Info:\n"; 
    ss << "- Terrain: ...\n" << "- Resources: ...\n";
    for (const auto& resource : constants::resourceTypeNames) {
        ss << "   * " << resource << ": ...\n";
    }
    ss << "- Building: ...\n" << "- Unit: ...";
    tileInfoText.setString(ss.str());
    tileInfoText.setCharacterSize(constants::infoLayerTextSize);
    tileInfoText.setFillColor(sf::Color::White);
    tileInfoText.setStyle(sf::Text::Bold);
    tileInfoText.setPosition(tileInfoBackground.getPosition() + sf::Vector2f(10.f, 10.f));
    window.draw(tileInfoText);
}

void ui::InfoLayerRenderer::Update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (isNextTurnClicked(window, mousePos)) {
            game_->NextTurn();
            DrawTo(window);
        }
    }
    // Update Tile selection here
}

bool ui::InfoLayerRenderer::isNextTurnClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos) {
    return nextTurnButton_.getGlobalBounds().contains(mousePos);
}