#include "ui/main_menu_renderer.hpp"

int ui::MainMenuRenderer::Initialize(const std::shared_ptr<sf::Font>& font, const sf::Vector2f& view_size) {

    // initialize sprite for background image
    if (!background_texture_.loadFromFile(constants::kBackgroundImagePath)) {
        std::cerr << "Failed to load background_texture_ in ui::MainMenuRenderer::Initialize\n";
        return EXIT_FAILURE;
    }
    background_texture_.setSmooth(true);
    background_rect_.setTexture(&background_texture_, true);
    background_rect_.setSize(view_size);
    

    // Initialize back button
    back_to_main_menu_button_ = ui::ClickableCircleShape(view_size, 15, 3, sf::Vector2f(0.1f, 0.2f), 270, sf::Vector2f(0,0));

    // Initialize texts and set their position in the main menu
    title_ = sf::Text(constants::kGameTitle, *font, 50);
    title_pos_ = sf::Vector2f(0.1f, 0.3f);
    title_.setPosition(view_size.x*title_pos_.x, view_size.y*title_pos_.y);

    // Initialize clickable texts in main menu
    load_game_button_ =    ui::ClickableText(view_size, "Load Game",          *font, sf::Vector2f(0.1f, 0.3f),   constants::kMainMenuClickablesSize, sf::Vector2f(0, 1.375f*constants::kMainMenuClickablesSize));
    new_game_button_ =     ui::ClickableText(view_size, "New Game",           *font, sf::Vector2f(0.1f, 0.3f),   constants::kMainMenuClickablesSize, sf::Vector2f(0, 2.5f*constants::kMainMenuClickablesSize));
    start_loaded_button_ = ui::ClickableText(view_size, "Load Selected Game", *font, sf::Vector2f(0.25f, 0.85f), constants::kMainMenuClickablesSize, sf::Vector2f(0, 0));
    start_new_button_ =    ui::ClickableText(view_size, "Start New Game",     *font, sf::Vector2f(0.1f, 0.3f),   constants::kMainMenuClickablesSize, sf::Vector2f(0, 0));

    // Initialize option selectors for the main menu
    std::vector<std::pair<std::string, int>> playerCountTexts {
        std::pair("Two players", 2), std::pair("Three players", 3), std::pair("Four players", 4) };
    selections_.emplace_back(playerCountTexts, font, 35, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(150.f, 100.f), view_size);

    std::vector<std::pair<std::string, int>> mapSizeTexts {
        std::pair("Small map", 10), std::pair("Normal map", 20), std::pair("Large map", 40) };
    selections_.emplace_back(mapSizeTexts, font, 35, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(150.f, 150.f), view_size);

    std::vector<std::pair<std::string, int>> deckTexts {
        std::pair("Deck 1", 1), std::pair("Deck 2", 2), std::pair("Deck 3", 3) };
    selections_.emplace_back(deckTexts, font, 35, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(150.f, 200.f), view_size);

    // Initialize game name text
    game_name_input_ = ui::TextInput(view_size, "Game Name: ", *font, sf::Vector2f(0.1f, 0.75f), sf::Vector2f(0, 0), 30);
    
    // Initialize save file selector
    save_file_selection_.Initialize(view_size, font);

    // reset the state of the object
    Reset();

    // Save each screens window size. Used for updating elements when screen is resized but when certain screen is not active.
    current_window_size_global_ = view_size;
    current_window_size_main_ = view_size;
    current_window_size_new_ = view_size;
    current_window_size_load_ = view_size;
    
    return 0;
}   

// Update the elements based on the event
int ui::MainMenuRenderer::Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event) {
    //current_state_ = new_state_;

    if (current_state_ == 1) {
        // new game
        game_name_input_.UpdateEvent(event);
        if (back_to_main_menu_button_.IsClicked(mousePos, event)) {
            new_state_ = 0;
        } else {
            // Update selector states
            for (auto& e : selections_) {
                e.Update(mousePos, event);
            }
        }
    } else if (current_state_ == 2) {
        // load game
        if (back_to_main_menu_button_.IsClicked(mousePos, event)) {
            new_state_ = 0;
            save_file_selection_.Reset();
        } else {
            save_file_selection_.Update(window, mousePos, event, start_loaded_button_.IsClicked(mousePos, event));
        }
    } else {
        // main menu
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

void ui::MainMenuRenderer::UpdateOutsideEventLoop(const sf::Vector2f& window_size, const sf::Vector2f& mouse_pos) {

    if (current_window_size_global_ != window_size) {
        background_rect_.setSize(window_size);
        current_window_size_global_ = window_size;
    }

    if (new_state_ == 1) {
        // new game

        // true if window has been resized, false if not
        bool resized = current_window_size_new_ != window_size;
        if (resized) {
            current_window_size_new_ = window_size;
            game_name_input_.UpdatePosition(window_size);
        }

        start_new_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        back_to_main_menu_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);

        for (auto& selector : selections_) {
            selector.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        }
    } else if (new_state_ == 2) {
        // load game

        // true if window has been resized, false if not
        bool resized = current_window_size_load_ != window_size;
        if (resized) current_window_size_load_ = window_size;

        start_loaded_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        back_to_main_menu_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        save_file_selection_.UpdateOutsideEventLoop(window_size, resized);
    } else {
        // main menu

        // true if window has been resized, false if not
        bool resized = current_window_size_main_ != window_size;
        if (resized) {
            current_window_size_main_ = window_size;
            title_.setPosition(window_size.x * title_pos_.x, window_size.y * title_pos_.y);
        }
        
        new_game_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        load_game_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
    }

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
        game_name_input_.DrawTo(window);
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