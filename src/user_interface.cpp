#include "ui/user_interface.hpp"

int ui::UserInterface::Initialize() {

    // Graphics init
    window_.create(sf::VideoMode(constants::kInitWindowWidth, constants::kInitWindowHeight), "StrategyGame");
    view_ = window_.getDefaultView();
    view_size_ = view_.getSize();

    // Initialize font with shared_ptr
    if (!font_->loadFromFile("./texture/times.ttf")) {
        std::cerr << "Failed to initialize font_ in ui::UserInterface constructor\n";
        return EXIT_FAILURE;
    }

    // Main menu init
    if (!main_menu_.Initialize(font_, view_size_)) {
        return EXIT_FAILURE;
    }

    return 0;
}

bool ui::UserInterface::PollEvent() {
    mouse_pos_ = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
    return window_.pollEvent(event_);
}

void ui::UserInterface::HandleGeneralEvents() {
    if (event_.type == sf::Event::Closed) window_.close();
    if (event_.type == sf::Event::Resized) {
        view_.setSize(sf::Vector2f(event_.size.width, event_.size.height));
        window_.setView(view_);
    }
}

void ui::UserInterface::HandleMainMenuEvents() {

    // if left mouse button is released
    if (event_.type == sf::Event::MouseButtonReleased && 
        event_.mouseButton.button == sf::Mouse::Left) {
        
        // update main menu elements that do something when LMB is released
        main_menu_.UpdateLMBReleased(window_);
    }

    // update main menu elements that do something when hovered over
    main_menu_.UpdateHovered(window_, mouse_pos_);
}

bool ui::UserInterface::IsPlayClicked() {
    return main_menu_.IsPlayClicked(window_, mouse_pos_, event_);
}

// TODO: temporarily use start variable
void ui::UserInterface::DrawAndDisplay(bool start) {
    // clear the screen
    window_.clear();
    // draw the sprites etc. in vertices_
    //window_.draw(vertices_);

    // TODO: temporary, update the underlying classes to use vertices_ instead
    if (!start) {
        main_menu_.DrawTo(window_);
    } else {
        map_renderer_.DrawTo(window_);
    }
    
    // Update the window
    window_.display();

    // empty the vertices array
    //vertices_.clear();
}