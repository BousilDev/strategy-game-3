#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <assert.h>
#include <vector>

#include "ui/selection.hpp"
#include "ui/center_origin.hpp"
#include "ui/clickable_text.hpp"
#include "ui/clickable_circle_shape.hpp"
#include "constants/constants.hpp"

namespace ui {


//TODO: group update methods into one method maybe? (might not be needed)
class MainMenuRenderer {
public:
    // Initialize a MainMenuRenderer object. Returns 0 if succesful and 1 if there are errors.
    // Errors are also printed to error stream
    int Initialize(const std::shared_ptr<sf::Font>& font, const sf::Vector2f& view_size);

    // Update the state of the main menu elements based on the event
    int Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event);

    // Check if start is clicked
    bool IsStartClicked(const sf::Vector2f& mousePos, const sf::Event& event) const;

    // Check if load is clicked
    bool IsLoadClicked(const sf::Vector2f& mousePos, const sf::Event& event) const;

    // Draw the sprites and selectors
    void DrawTo(sf::RenderWindow& window);

    //TODO: causes a segfault when indexing if passing vector by reference? currently in a safe state
    // return the options selected in selectors
    std::vector<int> GetSelectedOptions() const;

    // TODO: add other stuff if needed
    // reset the state. Used when returning to main menu from somewhere.
    void Reset() {
        current_state_ = 0;
        new_state_ = 0;
    }

private:
    sf::Texture background_texture_;
    sf::Sprite background_;

    //FIXME: handle these better, the main reason for these is the possibility to call IsPlayClicked after the internal state has been updated,
    //       which causes it to return true when it should just be in the new game menu
    int current_state_; // 0 main menu, 1 new game, 2 load game
    int new_state_;

    ui::ClickableCircleShape back_to_main_menu_button_;

    // main menu
    sf::Text title_;
    ui::ClickableText new_game_button_;
    ui::ClickableText load_game_button_;

    // new game
    ui::ClickableText start_new_button_;
    std::vector<ui::Selection> selections_;

    // load game
    ui::ClickableText start_loaded_button_;
};

} // namespace ui