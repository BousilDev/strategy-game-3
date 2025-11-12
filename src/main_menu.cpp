#include "ui/main_menu.hpp"

int ui::MainMenu::Initialize(const std::shared_ptr<sf::Font> font, sf::Vector2f view_size) {

    // initialize sprite for background image
    if (!background_texture_.loadFromFile(constants::kBackgroundImagePath)) {
        std::cerr << "Failed to load texture_ in ui::MainMenu::Initialize\n";
        return EXIT_FAILURE;
    }
    background_ = sf::Sprite(background_texture_);

    // Initialize texts and set their position in the main menu
    title_ = sf::Text("Strategy Game 3", *font, 50);
    title_.setPosition(view_size.x*0.1, view_size.y*0.1);

    // TODO: fix the positioning
    load_button_ = sf::Text("Load Game", *font, 40);
    load_button_.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    play_button_ = sf::Text("New Game", *font, 40);
    play_button_.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.3));

    //options_ = sf::Text("Options", *font, 35);
    //options_.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    // Initialize option selectors for the main menu
    std::vector<std::pair<std::string, int>> playerCountTexts {
        std::pair("Two players", 2), std::pair("Three players", 3), std::pair("Four players", 4) };
    selections_.emplace_back(playerCountTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.45));

    std::vector<std::pair<std::string, int>> mapSizeTexts {
        std::pair("Small map", 3), std::pair("Normal map", 5), std::pair("Large map", 7) };
    selections_.emplace_back(mapSizeTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.55));

    std::vector<std::pair<std::string, int>> deckTexts {
        std::pair("Deck 1", 1), std::pair("Deck 2", 2), std::pair("Deck 3", 3) };
    selections_.emplace_back(deckTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.65));

    return 0;
}   

// Update the elements based on the event
int ui::MainMenu::Update(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event) {
    // Left mouse button released
    if (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left) {
        for (auto& e : selections_) { e.UpdateState(window); }
    }

    // Make selector buttons slightly larger if mouse is hovering on them
    for (auto& e : selections_) {
        e.UpdateHovered(window);
    }

    // Make play button slightly larger if mouse is hovering on it
    if(play_button_.getGlobalBounds().contains(mousePos)) {
        play_button_.setScale(1.1,1.1);
    } else {
        play_button_.setScale(1,1);
    }

    if(load_button_.getGlobalBounds().contains(mousePos)) {
        load_button_.setScale(1.1,1.1);
    } else {
        load_button_.setScale(1,1);
    }
    
    return 0;
}

void ui::MainMenu::DrawTo(sf::RenderWindow& window) {
    window.draw(background_);
    for (auto e : selections_) {
        e.DrawTo(window);
    }
    window.draw(title_);
    window.draw(play_button_);
    //window.draw(options_);
    window.draw(load_button_);
}

bool ui::MainMenu::IsPlayClicked(const sf::RenderWindow& window, sf::Vector2f mouse_pos, sf::Event event) const {
    return (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left &&
        play_button_.getGlobalBounds().contains(mouse_pos));
}

bool ui::MainMenu::IsLoadClicked(const sf::RenderWindow& window, sf::Vector2f mouse_pos, sf::Event event) const {
    return (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left &&
        load_button_.getGlobalBounds().contains(mouse_pos));
}

// TODO: improve: currently returns vector with elements (playerCount, map, deck)
//                perhaps return a map?
std::vector<int> ui::MainMenu::GetSelectedOptions() {
    std::vector<int> selected_options;
    for (auto e : selections_) {
        selected_options.emplace_back(e.GetSelectedOption());
    }
    return selected_options;
}