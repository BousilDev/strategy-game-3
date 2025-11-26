#include "ui/main_menu_renderer.hpp"

int ui::MainMenuRenderer::Initialize(const std::shared_ptr<sf::Font>& font, const sf::Vector2f& view_size) {

    // initialize sprite for background image
    if (!background_texture_.loadFromFile(constants::kBackgroundImagePath)) {
        std::cerr << "Failed to load background_texture_ in ui::MainMenuRenderer::Initialize\n";
        return EXIT_FAILURE;
    }
    // TODO: background_texture_.setSmooth(true);
    //background_ = sf::Sprite(background_texture_);
    background_rect_.setTexture(&background_texture_, true);
    

    // Initialize back button
    back_to_main_menu_button_ = ui::ClickableCircleShape(15, 3, sf::Vector2f(view_size.x*0.1, view_size.y*0.2), 270);

    // Initialize texts and set their position in the main menu
    title_ = sf::Text(constants::kGameTitle, *font, 50);
    title_.setPosition(view_size.x*0.1, view_size.y*0.1);

    // TODO: fix the positioning
    load_game_button_ =    ui::ClickableText("Load Game",          *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.2),  constants::kMainMenuClickablesSize);
    new_game_button_ =     ui::ClickableText("New Game",           *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.3),  constants::kMainMenuClickablesSize);
    start_loaded_button_ = ui::ClickableText("Load Selected Game", *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.75), constants::kMainMenuClickablesSize);
    start_new_button_ =    ui::ClickableText("Start New Game",     *font, sf::Vector2f(view_size.x*0.1, view_size.y*0.3),  constants::kMainMenuClickablesSize);

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

    // Initialize save file selector
    save_file_selection_.Initialize(font);

    // reset the state of the object
    Reset();
    
    return 0;
}   

// Update the elements based on the event
int ui::MainMenuRenderer::Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event) {
    //current_state_ = new_state_;

    //TODO: resizing support
    if (event.type == sf::Event::Resized) {
        //window.getSize();
        //auto textureSize = background_texture_.getSize();
        //background_.setScale(
        //    window.getSize().x / static_cast<float>(textureSize.x),
        //    window.getSize().y / static_cast<float>(textureSize.y)
        //);
        //background_.setPosition(0.f, 0.f);

        background_rect_.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    }

    if (current_state_ == 1) {
    // new game  
        if (back_to_main_menu_button_.IsClicked(mousePos, event)) {
            new_state_ = 0;
        } else {
            start_new_button_.Update(window, mousePos, event);
            back_to_main_menu_button_.Update(window, mousePos, event);
            // Update selector states
            for (auto& e : selections_) {
                e.Update(window, mousePos, event);
            }
        }
    } else if (current_state_ == 2) {
    // load game
        if (back_to_main_menu_button_.IsClicked(mousePos, event)) {
            new_state_ = 0;
            save_file_selection_.Reset();
        } else {
            save_file_selection_.Update(window, mousePos, event, IsLoadClicked(mousePos, event));
            start_loaded_button_.Update(window, mousePos, event);
            back_to_main_menu_button_.Update(window, mousePos, event);
            
        }
    } else {
    // main menu
        new_game_button_.Update(window, mousePos, event);
        load_game_button_.Update(window, mousePos, event);

        if (new_game_button_.IsClicked(mousePos, event)) {
            new_state_ = 1;
        } else if (load_game_button_.IsClicked(mousePos, event)) {
            save_file_selection_.ScanFiles();
            new_state_ = 2;
            save_file_selection_.Update(window, mousePos, event, IsLoadClicked(mousePos, event));
        }
    }

    return 0;
}

void ui::MainMenuRenderer::DrawTo(sf::RenderWindow& window) {
    current_state_ = new_state_;
    window.draw(background_rect_);

    if (current_state_ == 1) {
        // new game
        start_new_button_.DrawTo(window);
        for (auto selection : selections_) {
            selection.DrawTo(window);
        }
        back_to_main_menu_button_.DrawTo(window);

    } else if (current_state_ == 2) {
        // load game
        start_loaded_button_.DrawTo(window);
        back_to_main_menu_button_.DrawTo(window);
        save_file_selection_.DrawTo(window);

    } else {
        // main menu
        window.draw(title_);
        new_game_button_.DrawTo(window);
        load_game_button_.DrawTo(window);
    }
}