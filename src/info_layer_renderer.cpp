#include "ui/info_layer_renderer.hpp"
#include <sstream>
#include <string>

// Initializes the InfoLayerRenderer
void ui::InfoLayerRenderer::Initialize(core::Game& game, const std::shared_ptr<sf::Font> font) {
    game_ = &game;
    font_ = font;

    sf::RectangleShape background;
    background.setFillColor(constants::infoLayerColor);
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(5.f);
    background_ = background;

    sf::RectangleShape nextTurnButton;
    nextTurnButton.setSize(sf::Vector2f(120.f, 40.f));
    nextTurnButton.setFillColor(sf::Color(100, 200, 100, 255));
    nextTurnButton.setOutlineColor(sf::Color::White);
    nextTurnButton.setOutlineThickness(2.f);
    nextTurnButton_ = nextTurnButton;

    sf::Text nextTurnText;
    nextTurnText.setFont(*font_);
    nextTurnText.setCharacterSize(constants::infoLayerTextSize);
    nextTurnText.setFillColor(sf::Color::Black);
    nextTurnText.setStyle(sf::Text::Bold);
    nextTurnText.setString("Next Turn");
    nextTurnText_ = nextTurnText;

    sf::RectangleShape tileInfoBackground;
    tileInfoBackground.setSize(sf::Vector2f(constants::kInitWindowWidth / 4.f, 2 * constants::kInitWindowHeight / 3.f));
    tileInfoBackground.setFillColor(constants::infoLayerColor);
    tileInfoBackground.setOutlineColor(sf::Color::White);
    tileInfoBackground.setOutlineThickness(5.f);
    tileInfoBackground_ = tileInfoBackground;

    sf::Text tileInfoText;
    tileInfoText.setFont(*font_);
    tileInfoText.setCharacterSize(constants::infoLayerTextSize);
    tileInfoText.setFillColor(sf::Color::White);
    tileInfoText.setStyle(sf::Text::Bold);
    tileInfoText_ = tileInfoText;

    sf::RectangleShape box;
    box.setFillColor(constants::infoLayerColor + sf::Color(100, 0, 0, 255));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.f);
    infoBackground_ = box;

    sf::Text infoText;
    infoText.setFont(*font_);
    infoText.setCharacterSize(constants::infoLayerTextSize);
    infoText.setFillColor(sf::Color::White);
    infoText.setStyle(sf::Text::Bold);
    infoText_ = infoText;

    sf::RectangleShape cardBackground;
    cardBackground.setFillColor(constants::infoLayerCardColor);
    cardBackground.setOutlineColor(sf::Color::White);
    cardBackground.setOutlineThickness(1.f);
    cardBackground.setSize(sf::Vector2f(constants::infoLayerCardWidth, constants::infoLayerCardHeight));
    cardBackground_ = cardBackground;
}
/*
void CopyTextProperties(sf::Text& source, sf::Text& target) {
    target.setFont(*source.getFont());
    target.setCharacterSize(source.getCharacterSize());
    target.setFillColor(source.getFillColor());
    target.setStyle(source.getStyle());
}  
*/

// Updates the info items
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
    sf::Text text = infoText_;
    text.setString(item.description + ": " + item.value);

    // Draw background box for the text
    sf::RectangleShape box = infoBackground_;
    box.setSize(sf::Vector2f(text.getLocalBounds().width + 10.f, constants::infoLayerTextSize + 5.f));
    box.setPosition(GetFixedPosition(window, location));
    window.draw(box);

    // Set text position to follow the text box position
    text.setPosition(box.getPosition().x + 5.f, box.getPosition().y);
    window.draw(text);
    return std::max(0, static_cast<int>(box.getLocalBounds().width + 10.f));
}

// Draws the given items to the upper info layer
void ui::InfoLayerRenderer::DrawItems(sf::RenderWindow& window, std::vector<ui::DrawItem<std::string>> items, int& textLen) {
    int maxTextLen = 0;
    int i = 0;
    for (const auto& item : items) {
        if (i < parts_) {
            maxTextLen = std::max(maxTextLen, DrawItemAtLocation(window, item, sf::Vector2f(10.f + textLen, 10.f + i * inc_)));
            i++;
        } else {
            textLen += maxTextLen;
            maxTextLen = DrawItemAtLocation(window, item, sf::Vector2f(10.f + textLen, 10.f));
            i = 1;
        }
    }
    textLen += maxTextLen;
}

std::string ui::InfoLayerRenderer::GetCardInfoString(std::shared_ptr<cards::Card> card) {
    std::stringstream ss;
    ss << card->GetName() << "\n";
    ss << card->GetDescription();
    return ss.str();
}

void ui::InfoLayerRenderer::DrawCardAtLocation(sf::RenderWindow& window, std::shared_ptr<cards::Card> card, sf::Vector2f location, int index) {
    cardBackgrounds_[index].setPosition(GetFixedPosition(window, location));
    // Change background color according to card type
    cardBackgrounds_[index].setFillColor(constants::infoLayerCardColor + sf::Color(0, static_cast<int>(card->GetCardType()) * 100, 0, 255));

    sf::Text cardText = tileInfoText_;
    cardText.setCharacterSize(constants::infoLayerTextSize / 2.f);
    std::string cardInfo = GetCardInfoString(card);
    cardText.setString(cardInfo);
    cardText.setPosition(GetFixedPosition(window, sf::Vector2f(location.x + 10.f, location.y + 10.f)));

    window.draw(cardBackgrounds_[index]);
    window.draw(cardText);
}

void ui::InfoLayerRenderer::DrawCards(sf::RenderWindow& window) {
    auto cards = game_->GetCurrentPlayer().GetHand()->GetCards();
    int cardsSize = cards.size();
    float cardInc = constants::infoLayerCardsWidth / float(cardsSize);
    int i = 0;
    for (const auto& card : cards) {
        // Check if existing background for this card already exists
        // TODO : Make this more efficient and less error prone
        if (cardBackgrounds_.size() <= i) {
            sf::RectangleShape cardBackground = cardBackground_;
            cardBackgrounds_.push_back(cardBackground);
        }
        DrawCardAtLocation(window, card, sf::Vector2f(constants::infoLayerCardsMargin + i * cardInc, constants::kInitWindowHeight - constants::infoLayerCardHeight), i);
        i += 1;
    }
    // Free unnecessary backgrounds
    while (cardBackgrounds_.size() > cardsSize) {
        cardBackgrounds_.pop_back();
    }
}

std::string ui::InfoLayerRenderer::GetTileInfoString() {
    std::stringstream ss;
    ss << "Selected Tile Info:\n"; 
    ss << "- Terrain: " << selected_tile_->get_terrain()->get_name() << "\n";
    ss << "- Resources:\n";

    std::list<core::Resource> resources = selected_tile_->get_terrain()->get_resources();
    for (const auto& resource : resources) {
        ss << "   * " << constants::resourceTypeNames[static_cast<int>(resource.type)]
           << ": " << resource.amount << "\n";
    }

    if (selected_tile_->get_building() != nullptr) {
        ss << "- Building: " 
           << constants::buildingTypeNames[static_cast<int>(selected_tile_->get_building()->GetType())] 
           << "\n";
    }

    ss << "- Unit: TODO";

    if (constants::debug) {
        ss << "\nDEBUG INFO:\n";
        ss << "- Tile Number: " << selected_tile_->get_tile_number() << "\n";
    }

    return ss.str();
}

// Draws the info layer, updates draw items on every call
void ui::InfoLayerRenderer::DrawTo(sf::RenderWindow& window) {
    UpdateDrawItems();
    sf::Vector2f winSize = window.getView().getSize();

    // Draw the background for the upper info bar
    background_.setSize(sf::Vector2f(winSize.x, constants::infoLayerHeight));
    // Keep background fixed relative to the view
    background_.setPosition(GetFixedPosition(window, sf::Vector2f(0.f, 0.f)));
    window.draw(background_);

    int textLen = 0;
    DrawItems(window, items_, textLen);
    // TODO: Draw a separator here between general info and resource info
    DrawItems(window, resource_items_, textLen);
    textLen += 10.f;

    // Draw next turn button to top right corner of the info layer
    nextTurnButton_.setPosition(GetFixedPosition(window, sf::Vector2f(10.f + textLen, constants::infoLayerHeight / 2.f - nextTurnButton_.getSize().y / 2.f)));
    window.draw(nextTurnButton_);

    // Draw next turn text over the button
    nextTurnText_.setPosition(nextTurnButton_.getPosition().x + 10.f, nextTurnButton_.getPosition().y + 5.f);
    window.draw(nextTurnText_);

    // Draw a layer to the right lower corner showing selected tile info
    tileInfoBackground_.setPosition(GetFixedPosition(window, sf::Vector2f(winSize.x - (tileInfoBackground_.getSize().x + 10.f), winSize.y - (tileInfoBackground_.getSize().y + 10.f))));
    window.draw(tileInfoBackground_);

    if (selected_tile_ != nullptr) {
        if (selected_card_ != nullptr) {
            std::string cardInfoString = GetCardInfoString(selected_card_);
            tileInfoText_.setString(cardInfoString);
        } else {
            std::string tileInfoString = GetTileInfoString();
            tileInfoText_.setString(tileInfoString);
        }
    }

    tileInfoText_.setPosition(tileInfoBackground_.getPosition() + sf::Vector2f(10.f, 10.f));
    window.draw(tileInfoText_);

    // Draw cards here
    DrawCards(window);
}

void ui::InfoLayerRenderer::Update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, std::shared_ptr<world::Tile> tile_pointer) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (isNextTurnClicked(window, mousePos)) {
            game_->NextTurn();
            DrawTo(window);
            return;
        }
        // Update Tile selection
        if (tile_pointer != nullptr) {
            selected_tile_ = tile_pointer;
            DrawTo(window);
            return;
        }
        std::shared_ptr<cards::Card> card = CardClicked(window, mousePos);
        if (card != nullptr) {
            selected_card_ = card;
            DrawTo(window);
        }
    }
}

bool ui::InfoLayerRenderer::isNextTurnClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos) {
    return nextTurnButton_.getGlobalBounds().contains(mousePos);
}

std::shared_ptr<cards::Card> ui::InfoLayerRenderer::CardClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos) {
    // Travel the cardBackgrounds_ vector in reverse since last cards are printed on top
    for (int i = cardBackgrounds_.size() - 1; i >= 0; i--) {
        if (cardBackgrounds_[i].getGlobalBounds().contains(mousePos)) {
            return game_->GetCurrentPlayer().GetHand()->GetCards()[i];
        }
    }
    return nullptr;
}