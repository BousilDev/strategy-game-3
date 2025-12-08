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

    if (constants::kUseCoolTitle) {
        // Initialize cool title animation
        cool_title_.Initialize(view_size, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(-25.f, -50.f));
    } else {
        // Initialize title text and set its position in the main menu
        title_ = sf::Text(constants::kGameTitle, *font, 50);
        title_pos_ = sf::Vector2f(0.1f, 0.3f);
        title_.setPosition(view_size.x*title_pos_.x, view_size.y*title_pos_.y);
    }
    
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
        std::pair("Small map", 3), std::pair("Normal map", 7), std::pair("Large map", 15) };
    selections_.emplace_back(mapSizeTexts, font, 35, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(150.f, 150.f), view_size);

    std::vector<std::pair<std::string, int>> deckTexts {
        std::pair("Beginner Deck", 1), std::pair("Balanced Deck", 2), std::pair("Aggressive Deck", 3) };
    selections_.emplace_back(deckTexts, font, 35, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(150.f, 200.f), view_size);

    // Initialize game name text
    game_name_input_ = ui::TextInput(view_size, "Game Name: ", *font, sf::Vector2f(0.1f, 0.3f), sf::Vector2f(0, 250.f), 30);

    // Initialize game over text
    game_over_text_ = sf::Text("Game Over", *font, 50);
    game_over_text_.setPosition(view_size.x*0.2f, view_size.y*0.45f);
    game_over_pos_ = game_over_text_.getPosition();

    game_over_background_ = sf::RectangleShape();
    game_over_background_.setSize(view_size);
    game_over_background_.setFillColor(sf::Color(0, 0, 0, 100));
    
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
int ui::MainMenuRenderer::Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, std::shared_ptr<bool> game_ended) {

    if (*game_ended.get()) {
        if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::KeyPressed) {
            *game_ended.get() = false;
        }
    } else {
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
    }

    return 0;
}

void ui::MainMenuRenderer::UpdateOutsideEventLoop(const sf::Vector2f& window_size, const sf::Vector2f& mouse_pos, const float delta_seconds) {

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

        game_name_input_.UpdateOutsideEventLoop(delta_seconds);

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
            if (!constants::kUseCoolTitle) {
                title_.setPosition(window_size.x * title_pos_.x, window_size.y * title_pos_.y);
            }
        }
        
        new_game_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        load_game_button_.UpdateOutsideEventLoop(window_size, mouse_pos, resized);
        if (constants::kUseCoolTitle) {
            cool_title_.UpdateOutsideEventLoop(window_size, delta_seconds, resized);
        }
    }

}

void ui::MainMenuRenderer::DrawTo(sf::RenderWindow& window, bool game_ended) {
    current_state_ = new_state_;
    window.draw(background_rect_);

    if (game_ended) {
        game_over_background_.setSize(sf::Vector2f(window.getSize()));
        window.draw(game_over_background_);
        window.draw(game_over_text_);
    } else {
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
            if (constants::kUseCoolTitle) {
                cool_title_.DrawTo(window);
            } else {
                window.draw(title_);
            }
            new_game_button_.DrawTo(window);
            load_game_button_.DrawTo(window);
        }
    }
}