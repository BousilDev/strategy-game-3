#pragma once
#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>
#include "center_origin.hpp"


// Displays and loops through selection options in the game menu screen
class Selection {
public:
    
    Selection() {   } 

    Selection(std::vector<std::pair<sf::Text, int>> options, sf::Vector2f pos) : options_(options){
        for (auto& v : options_) {
            centerOrigin(v.first);
            v.first.setPosition(pos);
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

    void UpdateState(const sf::RenderWindow& window) {
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if(rightArrow_.getGlobalBounds().contains(mousePos)) {
            selectedOption_ = (selectedOption_ + 1) % options_.size();
        }
        if(leftArrow_.getGlobalBounds().contains(mousePos)) {
            selectedOption_ = (selectedOption_ - 1) < 0 ? options_.size() - 1 : selectedOption_ - 1;
        }
    }

    void UpdateHovered(const sf::RenderWindow& window) {
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        if(rightArrow_.getGlobalBounds().contains(mousePos)) {
            rightArrow_.setScale(1.1, 1.1);
        } else {
            rightArrow_.setScale(1.0, 1.0);
        }

        if(leftArrow_.getGlobalBounds().contains(mousePos)) {
            leftArrow_.setScale(1.1, 1.1);
        } else {
            leftArrow_.setScale(1.0, 1.0);
        }
    }

    void DrawTo(sf::RenderTarget& window) const {
       
        window.draw(options_[selectedOption_].first);
        window.draw(leftArrow_);
        window.draw(rightArrow_);
    }

    void SetPosition(const sf::Vector2f& pos) {

    }

    int GetSelectedOption() const {
        return options_[selectedOption_].second;
    }

private:
    std::vector<std::pair<sf::Text, int>> options_;
    sf::CircleShape leftArrow_;
    sf::CircleShape rightArrow_;
    int selectedOption_ = 0;
};