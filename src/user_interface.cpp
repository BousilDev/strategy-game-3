#include "ui/user_interface.hpp"

int ui::UserInterface::Initialize(core::Game& game) {

    // Graphics init
    window_.create(sf::VideoMode(constants::kInitWindowWidth, constants::kInitWindowHeight), constants::kGameTitle);
    window_.setFramerateLimit(60);
    view_ = window_.getDefaultView();
    view_size_ = view_.getSize();

    // Initialize font with shared_ptr
    if (!font_->loadFromFile(constants::kFontPath)) {
        std::cerr << "Failed to initialize font_ in ui::UserInterface constructor\n";
        return EXIT_FAILURE;
    }

    info_layer_renderer_.Initialize(game, font_);

    // Main menu init
    if (main_menu_renderer_.Initialize(font_, view_size_)) {
        return EXIT_FAILURE;
    }

    return 0;
}

//bool ui::UserInterface::PollAndHandleEvent(bool start) {
//    bool event_found = PollEvent();
//    HandleEvent(start);
//    return event_found;
//}


bool ui::UserInterface::PollEvent() {
    if (window_.pollEvent(event_)) {
        mouse_pos_ = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        return true;
    } else {
        event_ = sf::Event();
        return false;
    }
}

void ui::UserInterface::HandleEvent(bool start) {
    // Handle general events
    if (event_.type == sf::Event::Closed) window_.close();
    if (event_.type == sf::Event::Resized) {
        view_.setSize(sf::Vector2f(event_.size.width, event_.size.height));
        view_.setCenter(view_.getSize() * 0.5f);
        view_size_ = view_.getSize();
        window_.setView(view_);
    }

    // Handle main menu events
    if (!start) {
        main_menu_renderer_.Update(window_, mouse_pos_, event_);
    } else {
        std::shared_ptr<world::Tile> tile_pointer = std::make_shared<world::Tile>(); // was nullptr
        
        if (event_.type == sf::Event::MouseButtonReleased && 
            event_.mouseButton.button == sf::Mouse::Left || event_.mouseButton.button == sf::Mouse::Right) {
            // update map elements that do something when LMB or RMB is released
            tile_pointer = map_renderer_.GetClickedTile(window_);
            if (tile_pointer != nullptr) {
                std::cout << "Tile number: " << tile_pointer->get_tile_number() 
                        << "\nTile terrain: " << tile_pointer->get_terrain()->get_name() 
                        <<"\ntiles <=2 away:" << std::endl;

                        // Get all tiles within distance 2
                        std::vector<unsigned int>& tiles_in_range = tile_pointer->get_tiles_in_n_range(2);

                        for (auto idx : tiles_in_range) {
                            std::cout << idx << " ";
                        }
                        std::cout << std::endl;
            }
        }

        info_layer_renderer_.Update(window_, mouse_pos_, event_, tile_pointer);
        map_renderer_.Update(window_, event_);
    }

}

// Returns nullptr if no tile is currently selected !!
std::shared_ptr<world::Tile> ui::UserInterface::GetLastClickedTile() {
  return map_renderer_.GetLastClickedTile();
}

bool ui::UserInterface::IsStartClicked() {
    return main_menu_renderer_.IsStartClicked(mouse_pos_, event_);
}

bool ui::UserInterface::IsLoadClicked() {
    return main_menu_renderer_.IsLoadClicked(mouse_pos_, event_);
}

// TODO: temporarily use start variable
void ui::UserInterface::DrawAndDisplay(bool start) {
    // clear the screen
    window_.clear(sf::Color(0,123,167));

    // render main menu if game has not started, otherwise render the map etc.
    if (!start) {
        main_menu_renderer_.DrawTo(window_);
    } else {
        map_renderer_.DrawTo(window_);
        info_layer_renderer_.DrawTo(window_);
    }
    
    // Update the window
    window_.display();
}

// Outside of event loop
void ui::UserInterface::UpdateOutsideEventLoop() {
    map_renderer_.PanMap(window_);
    map_renderer_.SetViewOnPlayer(window_);
}