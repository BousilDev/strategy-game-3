#include "ui/main_menu.hpp"

int ui::MainMenu::Initialize(const std::shared_ptr<sf::Font> font, sf::Vector2f view_size) {

    // initialize sprite for background image
    if (!texture_.loadFromFile("./texture/background.jpg")){
        return EXIT_FAILURE;
    }
    sprite_ = sf::Sprite(texture_);

    // Initialize texts and set their position in the main menu
    name_ = sf::Text("Placeholder", *font, 50);
    name_.setPosition(view_size.x*0.1, view_size.y*0.1);

    play_ = sf::Text("Play", *font, 40);
    play_.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    options_ = sf::Text("Options", *font, 35);
    options_.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    // Initialize option selectors for the main menu
    std::vector<std::pair<std::string, int>> playerCountTexts {
        std::pair("Two players", 2), std::pair("Three players", 3), std::pair("Four players", 4) };
    selections_.emplace_back(playerCountTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.35));

    std::vector<std::pair<std::string, int>> mapSizeTexts {
        std::pair("Small map", 3), std::pair("Normal map", 5), std::pair("Large map", 7) };
    selections_.emplace_back(mapSizeTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.45));

    std::vector<std::pair<std::string, int>> deckTexts {
        std::pair("Deck 1", 1), std::pair("Deck 2", 2), std::pair("Deck 3", 3) };
    selections_.emplace_back(deckTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.55));

    return 0;
}   

void ui::MainMenu::UpdateLMBReleased(const sf::RenderWindow& window) {
    for (auto& e : selections_) {
        e.UpdateState(window);
    }

    //playerCountSelection.UpdateState(window);
    //mapSelection.UpdateState(window);
    //deckSelection.UpdateState(window);
}

void ui::MainMenu::UpdateHovered(const sf::RenderWindow& window, sf::Vector2f mousePos) {
    for (auto e : selections_) {
        e.UpdateHovered(window);
    }

    //TODO: implement this better
    // Make play button slightly larger if mouse is hovering on it
    if(play_.getGlobalBounds().contains(mousePos)) {
        play_.setScale(1.1,1.1);
    } else {
        play_.setScale(1,1);
    }
}

void ui::MainMenu::DrawTo(sf::RenderWindow& window) {
    window.draw(sprite_);
    for (auto e : selections_) {
        e.DrawTo(window);
    }
    window.draw(name_);
    window.draw(play_);
}

// TODO: pass mousePos or something idk
bool ui::MainMenu::IsPlayClicked(const sf::RenderWindow& window, sf::Vector2f mousePos) {
    if (play_.getGlobalBounds().contains(mousePos)) {
        return true;   
    } else {
        return false;
    }
}

// TODO: improve: currently returns vector with elements (playerCount, map, deck)
// perhaps return a map?
// FIXME: this causes a segmentation fault
std::vector<int> ui::MainMenu::GetSelectedOptions() {
    std::vector<int> selected_options;
    for (auto e : selections_) {
        selected_options.emplace_back(e.GetSelectedOption());
    }
    return selected_options;
}