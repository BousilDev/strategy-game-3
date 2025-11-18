#pragma once

#include <SFML/Graphics.hpp>

#include "constants/constants.hpp"

namespace ui {

// a wrapper for sf::Text objects that do something when clicked and change size when hovered over.
class ClickableText {
public:

    ClickableText() {}

    ClickableText(const sf::String &string, const sf::Font &font, sf::Vector2f position, unsigned int characterSize = 30U,
                  sf::Vector2f hovered_scale = constants::kClickableTextHoveredScale)
                  : position_(position), hovered_scale_(hovered_scale) {
        text_ = sf::Text(string, font, characterSize);
        text_.setPosition(position_);
    }

    void Update(const sf::RenderWindow& window, const sf::Vector2f& mouse_pos, const sf::Event& event) {
        // Resized
        if (event.type == sf::Event::Resized) {
            //TODO: implement resizing support
        }

        // Hovered over
        if (text_.getGlobalBounds().contains(mouse_pos)) {
            text_.setScale(hovered_scale_);
        } else {
            text_.setScale(1,1);
        }
    }

    bool IsClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
        return (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left &&
                text_.getGlobalBounds().contains(mouse_pos));
    }

    void DrawTo(sf::RenderWindow& window) {
        window.draw(text_);
    }

private:
    sf::Text text_;
    sf::Vector2f position_;
    sf::Vector2f hovered_scale_;
};

} // namespace ui