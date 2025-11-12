#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ui/main_menu.hpp"
#include "ui/map_renderer.hpp"
#include "constants/constants.hpp"

namespace ui {


class UserInterface {
public:

    int Initialize();

    //FIXME: using this in the event handling loop in main causes a weird bug
    // Update mousepos, poll next event and handle it
    //bool PollAndHandleEvent();

    // Update mouse position and poll events. Returns true if event found.
    bool PollEvent();

    // Handle current event
    void HandleEvent();

    // check if play button is clicked
    bool IsPlayClicked();

    // check if load button is clicked
    bool IsLoadClicked();

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
    sf::Vector2f mouse_pos_;
    sf::Event event_;
    std::shared_ptr<sf::Font> font_ = std::make_shared<sf::Font>();
    ui::MainMenu main_menu_;
    ui::MapRenderer map_renderer_;
};

} // namespace ui