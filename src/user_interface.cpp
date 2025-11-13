#include "ui/user_interface.hpp"

int ui::UserInterface::Initialize(core::Game& game) {

    // Graphics init
    window_.create(sf::VideoMode(constants::kInitWindowWidth, constants::kInitWindowHeight), constants::kGameTitle);
    view_ = window_.getDefaultView();
    view_size_ = view_.getSize();

    // Initialize font with shared_ptr
    if (!font_->loadFromFile(constants::kFontPath)) {
        std::cerr << "Failed to initialize font_ in ui::UserInterface constructor\n";
        return EXIT_FAILURE;
    }

    info_layer_renderer_ = std::make_unique<ui::InfoLayerRenderer>(game, font_);

    // Main menu init
    if (!main_menu_.Initialize(font_, view_size_)) {
        return EXIT_FAILURE;
    }

    return 0;
}

//bool ui::UserInterface::PollAndHandleEvent() {
//    bool event_found = PollEvent();
//    HandleEvent();
//    return event_found;
//}


bool ui::UserInterface::PollEvent() {
    mouse_pos_ = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
    return window_.pollEvent(event_);
}

// TODO: Temp event getter thing for map rendering soz
sf::Event& ui::UserInterface::GetEvent() {
    return event_;
}

void ui::UserInterface::HandleEvent(bool start) {
    // Handle general events
    if (event_.type == sf::Event::Closed) window_.close();
    if (event_.type == sf::Event::Resized) {
        view_.setSize(sf::Vector2f(event_.size.width, event_.size.height));
        window_.setView(view_);
    }

    // Handle main menu events
    if (!start) {
        main_menu_.Update(window_, mouse_pos_, event_);
    } else {
        info_layer_renderer_->Update(window_, mouse_pos_, event_);
        if (event_.type == sf::Event::MouseButtonReleased && 
            event_.mouseButton.button == sf::Mouse::Left) {
        // update map elements that do something when LMB is released
            auto tile_pointer = map_renderer_.GetClickedTile(window_);
            if (tile_pointer != nullptr) {
                std::cout << "Tile number: " << tile_pointer->get_tile_number() 
                        << "\nTile terrain: " << tile_pointer->get_terrain()->get_name() << std::endl;
            }
        } 
    }

}

bool ui::UserInterface::IsPlayClicked() {
    return main_menu_.IsPlayClicked(window_, mouse_pos_, event_);
}

bool ui::UserInterface::IsLoadClicked() {
    return main_menu_.IsLoadClicked(window_, mouse_pos_, event_);
}

// TODO: temporarily use start variable
void ui::UserInterface::DrawAndDisplay(bool start) {
    // clear the screen
    window_.clear();

    // render main menu if game has not started, otherwise render the map etc.
    if (!start) {
        main_menu_.DrawTo(window_);
    } else {
        map_renderer_.DrawTo(window_);
        info_layer_renderer_->DrawTo(window_);
    }
    
    // Update the window
    window_.display();
}