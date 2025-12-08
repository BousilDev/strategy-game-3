#pragma once

/**
 * @file clickable_text.hpp
 * @brief Declares the ClickableText class which is a wrapper for sf::Text that handles clicks and hover effects.
 */

#include <SFML/Graphics.hpp>

#include "constants/constants.hpp"

namespace ui {

/**
 * @class ClickableText
 * @brief A wrapper for sf::Text that handles clicks and hover effects.
 */
class ClickableText {
public:

    /**
     * @brief Default constructor.
     */
    ClickableText() {}

    /**
     * @brief Construct a ClickableText instance.
     * 
     * @param viewSize The size of the view.
     * @param string The text string.
     * @param font The font used for the text.
     * @param position The relative position (0.0 to 1.0) of the text in the window.
     * @param characterSize The character size of the text.
     * @param offset The pixel offset from the relative position.
     * @param hovered_scale The scale to apply when the text is hovered over.
     */
    ClickableText(const sf::Vector2f& viewSize, const sf::String& string, const sf::Font& font, const sf::Vector2f& position, unsigned int characterSize = 30U,
                  sf::Vector2f offset = sf::Vector2f(0, 0), sf::Vector2f hovered_scale = sf::Vector2f(1.1f, 1.1f))
                  : position_(position), offset_(offset), hovered_scale_(hovered_scale) {
        text_ = sf::Text(string, font, characterSize);
        text_.setPosition(position_.x*viewSize.x + offset_.x, position_.y*viewSize.y + offset_.y);
    }

    /**
     * @brief Update the ClickableText instance outside the event loop.
     * 
     * @param viewSize The size of the view.
     * @param mouse_pos The mouse position.
     * @param resized Whether the window was resized.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& viewSize, const sf::Vector2f& mouse_pos, const bool& resized) {
        if (resized) {
            text_.setPosition(position_.x*viewSize.x + offset_.x, position_.y*viewSize.y + offset_.y);
        }

        if (text_.getGlobalBounds().contains(mouse_pos)) {
            text_.setScale(hovered_scale_);
        } else {
            text_.setScale(1, 1);
        }
    }

    /**
     * @brief Check if the text is clicked.
     * 
     * @param mouse_pos The mouse position.
     * @param event The current event.
     * @return true If the text is clicked.
     * @return false Otherwise.
     */
    bool IsClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
        return (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left &&
                text_.getGlobalBounds().contains(mouse_pos));
    }

    /**
     * @brief Draw the ClickableText instance to the given window.
     * 
     * @param window The render window.
     */
    void DrawTo(sf::RenderWindow& window) {
        window.draw(text_);
    }

private:
    sf::Text text_;
    sf::Vector2f position_;
    sf::Vector2f hovered_scale_;
    sf::Vector2f offset_;
};

} // namespace ui