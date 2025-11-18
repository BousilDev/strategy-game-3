#include "ui/main_menu_renderer.hpp"

int ui::MainMenuRenderer::Initialize(const std::shared_ptr<sf::Font> font, sf::Vector2f view_size) {

    // initialize sprite for background image
    if (!background_texture_.loadFromFile(constants::kBackgroundImagePath)) {
        std::cerr << "Failed to load texture_ in ui::MainMenuRenderer::Initialize\n";
        return EXIT_FAILURE;
    }
    background_ = sf::Sprite(background_texture_);

    // Initialize back button
    back_to_main_menu_button_ = ui::ClickableCircleShape(15, 3, sf::Vector2f(view_size.x*0.1, view_size.y*0.2), 270);

    // Initialize texts and set their position in the main menu
    title_ = sf::Text(constants::kGameTitle, *font, 50);
    title_.setPosition(view_size.x*0.1, view_size.y*0.1);

    // TODO: fix the positioning
    load_game_button_ =    ui::ClickableText("Load Game",          *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.2), 40);
    new_game_button_ =     ui::ClickableText("New Game",           *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.3), 40);
    start_loaded_button_ = ui::ClickableText("Load Selected Game", *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.3), 40);
    start_new_button_ =    ui::ClickableText("Start New Game",     *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.3), 40);

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

    // reset the state of the object
    Reset();
    
    return 0;
}   

// Update the elements based on the event
int ui::MainMenuRenderer::Update(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event) {
    state_ = new_state_;

    if (state_ == 1) {
    // new game
        start_new_button_.Update(window, mousePos, event);
        back_to_main_menu_button_.Update(window, mousePos, event);
    
        // Update selector states
        for (auto& e : selections_) {
            e.Update(window, mousePos, event);
        }

        if (back_to_main_menu_button_.IsClicked(mousePos, event)) {
            new_state_ = 0;
        }

    } else if (state_ == 2) {
    // load game
        start_loaded_button_.Update(window, mousePos, event);
        back_to_main_menu_button_.Update(window, mousePos, event);

        if (back_to_main_menu_button_.IsClicked(mousePos, event)) {
            new_state_ = 0;
        }
    } else {
    // main menu
        new_game_button_.Update(window, mousePos, event);
        load_game_button_.Update(window, mousePos, event);

        if (new_game_button_.IsClicked(mousePos, event)) {
            new_state_ = 1;
        } else if (load_game_button_.IsClicked(mousePos, event)) {
            new_state_ = 2;
        }
    }

    return 0;
}

void ui::MainMenuRenderer::DrawTo(sf::RenderWindow& window) {
    if (state_ == 1) {
        // new game
        window.draw(background_);
        start_new_button_.DrawTo(window);
        for (auto selection : selections_) {
            selection.DrawTo(window);
        }
        back_to_main_menu_button_.DrawTo(window);

    } else if (state_ == 2) {
        // load game
        window.draw(background_);
        start_loaded_button_.DrawTo(window);
        back_to_main_menu_button_.DrawTo(window);

    } else {
        // main menu
        window.draw(background_);
        window.draw(title_);
        new_game_button_.DrawTo(window);
        load_game_button_.DrawTo(window);
    }
}

bool ui::MainMenuRenderer::IsStartClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
    return (state_ == 1 && start_new_button_.IsClicked(mouse_pos, event));
}

bool ui::MainMenuRenderer::IsLoadClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
    return (state_ == 2 && start_loaded_button_.IsClicked(mouse_pos, event));
}

// TODO: improve: currently returns vector with elements (playerCount, map, deck)
//                perhaps return a map?
std::vector<int> ui::MainMenuRenderer::GetSelectedOptions() {
    std::vector<int> selected_options;
    for (auto e : selections_) {
        selected_options.emplace_back(e.GetSelectedOption());
    }
    return selected_options;
}