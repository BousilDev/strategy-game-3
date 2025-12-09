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
     * @param view_size The size of the view.
     * @param string The text string.
     * @param font The font used for the text.
     * @param position The relative position (0.0 to 1.0) of the text in the window.
     * @param character_size The character size of the text.
     * @param offset The pixel offset from the relative position. Default is (0,0).
     */
    ClickableText(const sf::Vector2f& view_size, const sf::String& string, const sf::Font& font, const sf::Vector2f& position, unsigned int character_size = 30U,
                  sf::Vector2f offset = sf::Vector2f(0, 0)) : position_(position), offset_(offset) {
        text_ = sf::Text(string, font, character_size);
        text_.setPosition(position_.x*view_size.x + offset_.x, position_.y*view_size.y + offset_.y);
        text_.setFillColor(normal_color_);
        text_.setOutlineColor(outline_color_);
        text_.setOutlineThickness(outline_size_);
    }

    /**
     * @brief Update the ClickableText instance outside the event loop.
     * 
     * @param view_size The size of the view.
     * @param mouse_pos The mouse position.
     * @param resized Whether the window was resized.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& view_size, const sf::Vector2f& mouse_pos, const bool& resized) {
        if (resized) {
            text_.setPosition(position_.x*view_size.x + offset_.x, position_.y*view_size.y + offset_.y);
        }

        if (text_.getGlobalBounds().contains(mouse_pos)) {
            text_.setScale(hovered_scale_);
            text_.setFillColor(hovered_color_);
        } else {
            text_.setScale(1, 1);
            text_.setFillColor(normal_color_);
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
    sf::Vector2f position_;
    sf::Vector2f offset_;
    sf::Text text_;
    sf::Vector2f hovered_scale_ = constants::kClickableTextHoveredScale;
    sf::Color normal_color_ = constants::kClickableNormalColor;
    sf::Color hovered_color_ = constants::kClickableHoveredColor;
    sf::Color outline_color_ = constants::kClickableOutlineColor;
    float outline_size_ = constants::kClickableOutlineSize;
};

} // namespace ui