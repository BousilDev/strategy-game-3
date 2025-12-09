#pragma once

/**
 * @file selection.hpp
 * @brief Declares the Selection class which is used for the ui elements for the selectors
 */

#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>

#include "constants/constants.hpp"
#include "center_origin.hpp"

namespace ui {

/**
 * @class Selection
 * @brief Displays and loops through selection options in the game menu screen
 */
class Selection {
public:
    
    /**
     * @brief Construct a new Selection instance.
     * 
     * @param texts a vector containing the texts for the selections and the number for the action the selection does
     * @param font the font used for the texts
     * @param font_size the size of the texts
     * @param pos sets the position
     */
    Selection(const std::vector<std::pair<std::string, int>>& texts, const std::shared_ptr<sf::Font> font, const int font_size,
              const sf::Vector2f& pos, const sf::Vector2f& offset, const sf::Vector2f& window_size)
              : texts_(texts), pos_(pos), offset_(offset) {

        for (const auto& text : texts_) {
            auto currentText = std::pair(sf::Text(text.first, *font, font_size), text.second);
            centerOrigin(currentText.first);
            currentText.first.setPosition(pos.x*window_size.x + offset_.x, pos.y*window_size.y + offset_.y);
            currentText.first.setOutlineColor(text_outline_color);
            currentText.first.setOutlineThickness(text_outline_size_);
            text_options_.push_back(currentText);
        }

        leftArrow_.setPointCount(3);
        leftArrow_.setRadius(15.f);
        centerOrigin(leftArrow_);
        leftArrow_.setRotation(270);
        leftArrow_.setPosition(pos.x * window_size.x - 150 + offset.x, pos.y * window_size.y + offset.y);
        leftArrow_.setFillColor(normal_color_);
        leftArrow_.setOutlineColor(outline_color_);
        leftArrow_.setOutlineThickness(clickable_outline_size_);

        rightArrow_.setPointCount(3);
        rightArrow_.setRadius(15.f);
        centerOrigin(rightArrow_);
        rightArrow_.setRotation(90);
        rightArrow_.setPosition(pos.x * window_size.x + 150 + offset.x, pos.y * window_size.y + offset.y);
        rightArrow_.setFillColor(normal_color_);
        rightArrow_.setOutlineColor(outline_color_);
        rightArrow_.setOutlineThickness(clickable_outline_size_);
    }

    /**
     * @brief Update the state of the object.
     * 
     * @param window a reference to a window object
     * @param mouse_pos an sf::Vector2f containing the mouse coordinates
     * @param event an sf::Event
     */
    void Update(const sf::Vector2f& mouse_pos, const sf::Event& event) {
        // if LMB is released on the selector
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (leftArrow_.getGlobalBounds().contains(mouse_pos)) {
                selectedOption_ = (selectedOption_ - 1) < 0 ? (text_options_.size() - 1) : (selectedOption_ - 1);
            }
            if (rightArrow_.getGlobalBounds().contains(mouse_pos)) {
                selectedOption_ = (selectedOption_ + 1) % text_options_.size();
            }
        }
    }

    /**
     * @brief Update the Selection instance outside the event loop.
     * 
     * @param window_size The window size.
     * @param mouse_pos The mouse position.
     * @param resized Whether the window was resized.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const sf::Vector2f& mouse_pos, const bool resized) {
        if (resized) {
            for (auto& text : text_options_) {
                centerOrigin(text.first);
                text.first.setPosition(sf::Vector2f(pos_.x*window_size.x + offset_.x, pos_.y*window_size.y + offset_.y));
            }
            centerOrigin(leftArrow_);
            leftArrow_.setPosition(pos_.x*window_size.x - 150 + offset_.x, pos_.y*window_size.y + offset_.y);
            centerOrigin(rightArrow_);
            rightArrow_.setPosition(pos_.x*window_size.x + 150 + offset_.x, pos_.y*window_size.y + offset_.y);
        }

        // if mouse hovers over
        if(leftArrow_.getGlobalBounds().contains(mouse_pos)) {
            leftArrow_.setScale(1.3, 1.3);
            leftArrow_.setFillColor(hovered_color_);
        } else {
            leftArrow_.setScale(1.0, 1.0);
            leftArrow_.setFillColor(normal_color_);
        }

        if(rightArrow_.getGlobalBounds().contains(mouse_pos)) {
            rightArrow_.setScale(1.3, 1.3);
            rightArrow_.setFillColor(hovered_color_);
        } else {
            rightArrow_.setScale(1.0, 1.0);
            rightArrow_.setFillColor(normal_color_);
        }
    }

    /**
     * @brief Draw the selector to the window
     * 
     * @param window a reference to a window object
     */
    void DrawTo(sf::RenderWindow& window) {
       
        window.draw(text_options_.at(selectedOption_).first);
        window.draw(leftArrow_);
        window.draw(rightArrow_);
    }

    /**
     * @brief Return the number of the selected option
     * 
     * @return selectedOption_
     */
    int GetSelectedOption() const {
        return text_options_.at(selectedOption_).second;
    }

private:
    std::vector<std::pair<std::string, int>> texts_;
    std::vector<std::pair<sf::Text, int>> text_options_;
    sf::CircleShape leftArrow_;
    sf::CircleShape rightArrow_;
    int selectedOption_ = 0;
    sf::Vector2f pos_;
    sf::Vector2f offset_;
    sf::Color normal_color_ = constants::kClickableNormalColor;
    sf::Color hovered_color_ = constants::kClickableHoveredColor;
    sf::Color outline_color_ = constants::kClickableOutlineColor;
    float clickable_outline_size_ = constants::kClickableOutlineSize;
    sf::Color text_outline_color = constants::kSelectionTextOutlineColor;
    float text_outline_size_ = constants::kSelectionTextOutlineSize;
};

} // namespace ui
