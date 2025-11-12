#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <assert.h>
#include <vector>

#include "ui/selection.hpp"
#include "ui/center_origin.hpp"

namespace ui {


//TODO: group update methods into one method maybe? (might not be needed)
class MainMenu {
public:
    // TODO: add numbers to indicate success etc
    int Initialize(const std::shared_ptr<sf::Font> font, sf::Vector2f view_size);

    //TODO: group the update methods under this
    int Update(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event);

    // Update the state of the elements that change when left mouse button is released
    void UpdateLMBReleased(const sf::RenderWindow& window);

    // Update the state of the elements that change when hovered over
    void UpdateHovered(const sf::RenderWindow& window, sf::Vector2f mousePos);

    // Initialize game on pressing "play"
    bool IsPlayClicked(const sf::RenderWindow& window, sf::Vector2f mousePos, sf::Event event);




    // draw the sprites and selectors
    void DrawTo(sf::RenderWindow& window);

    //TODO: implement if you have time, this would improve performance by reducing draw calls
    // return a VertexArray containing the vertices that will be drawn
    //sf::VertexArray VerticesToDraw();

    // return the options selected in selectors
    std::vector<int> GetSelectedOptions();


private:
    std::vector<ui::Selection> selections_;
    sf::Text name_;
    sf::Text play_;
    sf::Text options_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

} // namespace ui