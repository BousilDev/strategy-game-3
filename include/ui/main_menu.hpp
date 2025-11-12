#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <assert.h>
#include <vector>

#include "ui/selection.hpp"
#include "ui/center_origin.hpp"
#include "constants/constants.hpp"

namespace ui {


//TODO: group update methods into one method maybe? (might not be needed)
class MainMenu {
public:
    // Initialize a MainMenu object. Returns 0 if succesful and 1 if there are errors.
    // Errors are also printed to error stream
    int Initialize(const std::shared_ptr<sf::Font> font, sf::Vector2f view_size);

    //TODO: maybe use the return value to replace isPlayClicked etc?
    // Update the state of the main menu elements based on the event
    int Update(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event);

    // Check if play is clicked
    bool IsPlayClicked(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event) const;

    // Check if load is clicked
    bool IsLoadClicked(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event) const;

    // Draw the sprites and selectors
    void DrawTo(sf::RenderWindow& window);

    // return the options selected in selectors
    std::vector<int> GetSelectedOptions();


private:
    std::vector<ui::Selection> selections_;
    sf::Text title_;
    sf::Text play_button_;
    sf::Text load_button_;
    //sf::Text options_;
    sf::Texture background_texture_;
    sf::Sprite background_;
};

} // namespace ui