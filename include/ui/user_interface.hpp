#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <ui/main_menu.hpp>
#include <ui/map_renderer.hpp>

namespace ui {


class UserInterface {
public:

    UserInterface();

    // update mouse position and poll events. Returns true if event found.
    bool PollEvent();

    // Handle events that do not depend on game state etc.
    void HandleGeneralEvents();

    void HandleMainMenuEvents();

    // check if play button is clicked
    bool IsPlayClicked();

    void InitializeMapRenderer(world::Map map) {
        map_renderer_.Initialize(map, window_);
    }

    // clears the window and draws sprites
    void DrawAndDisplay(bool start);

    sf::RenderWindow& GetWindow() { return window_; }

    // return the options selected in main menu selectors
    std::vector<int> GetSelectedOptions() { return main_menu_.GetSelectedOptions(); }

private:
    sf::RenderWindow window_;
    sf::View view_;
    sf::Vector2f view_size_;
    std::shared_ptr<sf::Font> font_ = std::make_shared<sf::Font>();
    sf::Vector2f mouse_pos_;
    sf::Event event_;


    sf::VertexArray vertices_;

    ui::MainMenu main_menu_;
    ui::MapRenderer map_renderer_;

};

} // namespace ui