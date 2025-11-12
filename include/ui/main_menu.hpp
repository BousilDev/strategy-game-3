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
    // Errors are also printed to
    int Initialize(const std::shared_ptr<sf::Font> font, sf::Vector2f view_size);

    //TODO: group the update methods under this?
    //int Update(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event);

    // Update the state of the elements that change when left mouse button is released
    void UpdateLMBReleased(const sf::RenderWindow& window);

    // Update the state of the elements that change when hovered over
    void UpdateHovered(const sf::RenderWindow& window, sf::Vector2f mousePos);

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
    std::vector<sf::Text> texts_;
    sf::Text name_;
    sf::Text play_;
    sf::Text load_;
    sf::Text options_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

} // namespace ui