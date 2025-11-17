#pragma once

/**
 * @file selection.hpp
 * @brief Declares the Selection class which is used for the ui elements for the selectors
 */

#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>

#include "center_origin.hpp"

namespace ui {

/**
 * @class Selection
 * @brief Displays and loops through selection options in the game menu screen
 */
class Selection {
public:
    
    // TODO: check that the shared pointer works correctly
    /**
     * @brief Construct a new Selection object
     * 
     * @param texts a vector containing the texts for the selections and the number for the action the selection does
     * @param font the font used for the texts
     * @param font_size the size of the texts
     * @param pos sets the position
     */
    Selection(const std::vector<std::pair<std::string, int>>& texts, const std::shared_ptr<sf::Font> font, int font_size, sf::Vector2f pos) : texts_(texts) {
          
        for (auto text : texts_) {
            auto currentText = std::pair(sf::Text(text.first, *font, font_size), text.second);
            centerOrigin(currentText.first);
            currentText.first.setPosition(pos);
            text_options_.push_back(currentText);
        }

        leftArrow_.setPointCount(3);
        leftArrow_.setRadius(15.f);
        centerOrigin(leftArrow_);
        leftArrow_.setRotation(270);
        leftArrow_.setPosition(pos.x-150,pos.y);

        rightArrow_.setPointCount(3);
        rightArrow_.setRadius(15.f);
        centerOrigin(rightArrow_);
        rightArrow_.setRotation(90);
        rightArrow_.setPosition(pos.x+150,pos.y);
    }

    /**
     * @brief Update the state of the object.
     * 
     * @param window a reference to a window object
     */
    void UpdateState(const sf::RenderWindow& window) {
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if(rightArrow_.getGlobalBounds().contains(mousePos)) {
            selectedOption_ = (selectedOption_ + 1) % text_options_.size();
        }
        if(leftArrow_.getGlobalBounds().contains(mousePos)) {
            selectedOption_ = (selectedOption_ - 1) < 0 ? text_options_.size() - 1 : selectedOption_ - 1;
        }
    }

    /**
     * @brief Update the state of the obect when mouse hovers over.
     * 
     * @param window a reference to a window object
     */
    void UpdateHovered(const sf::RenderWindow& window) {
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        if(rightArrow_.getGlobalBounds().contains(mousePos)) {
            rightArrow_.setScale(1.3, 1.3);
        } else {
            rightArrow_.setScale(1.0, 1.0);
        }

        if(leftArrow_.getGlobalBounds().contains(mousePos)) {
            leftArrow_.setScale(1.3, 1.3);
        } else {
            leftArrow_.setScale(1.0, 1.0);
        }
    }

    /**
     * @brief Draw the selector to the window
     * 
     * @param window a reference to a window object
     */
    void DrawTo(sf::RenderTarget& window) const {
       
        window.draw(text_options_.at(selectedOption_).first);
        window.draw(leftArrow_);
        window.draw(rightArrow_);
    }

    // TODO: implement if needed
    //void SetPosition(const sf::Vector2f& pos) {
    //
    //}

    /**
     * @brief Return the index of the selected option
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
};

} // namespace ui
