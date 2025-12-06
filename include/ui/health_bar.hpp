#pragma once
#include <SFML/Graphics.hpp>

// Health bar class draws health bars for units and buildings
class HealthBar {
public:
    HealthBar(sf::Vector2f position, int maxHp, int currentHp, bool isUnit) {
        // Background
        float offset = isUnit ? baseOffset_ : -baseOffset_ - 5.f;
        background_.setSize(sf::Vector2f(4.f, height_));
        background_.setFillColor(sf::Color(0, 255, 0));
        background_.setPosition(position + sf::Vector2f(offset, -height_ / 2));
        background_.setOutlineColor(sf::Color(0, 0, 0));
        background_.setOutlineThickness(1.f);

        // Health bar
        float missing_ratio = static_cast<float>(maxHp - currentHp) / static_cast<float>(maxHp);
        bar_.setSize(sf::Vector2f(4.f, height_ * missing_ratio));
        bar_.setFillColor(sf::Color(255, 0, 0));
        bar_.setPosition(position + sf::Vector2f(offset, -height_ / 2));
    }

    void DrawTo(sf::RenderWindow& window) {
        window.draw(background_);
        window.draw(bar_);
    }
private:
    sf::RectangleShape background_;
    sf::RectangleShape bar_;
    float height_ = 40.f;
    float baseOffset_ = 20.f;
};