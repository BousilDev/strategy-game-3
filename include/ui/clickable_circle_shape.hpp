#pragma once

#include <SFML/Graphics.hpp>

#include "constants/constants.hpp"
#include "ui/center_origin.hpp"

namespace ui {

// a wrapper for sf::CircleShape objects that do something when clicked and change size when hovered over.
class ClickableCircleShape {
public:

    ClickableCircleShape() {}

    ClickableCircleShape(const sf::Vector2f& view_size, float radius, std::size_t point_count, sf::Vector2f position,
                         float angle = 0, sf::Vector2f offset = sf::Vector2f(0,0), sf::Vector2f hovered_scale = sf::Vector2f(1.3f, 1.3f))
                        : position_(position), offset_(offset), hovered_scale_(hovered_scale) {
        circle_shape_ = sf::CircleShape(radius, point_count);
        centerOrigin(circle_shape_);
        circle_shape_.setRotation(angle);
        circle_shape_.setPosition(position_.x*view_size.x + offset_.x, position.y*view_size.y + offset_.y);
    }

    void Update(const sf::RenderWindow& window, const sf::Vector2f& mouse_pos, const sf::Event& event) {
        // Resized
        if (event.type == sf::Event::Resized) {
            sf::Vector2f window_size = sf::Vector2f(window.getSize().x, window.getSize().y);
            //TODO: implement resizing support
            circle_shape_.setPosition(position_.x*window_size.x + offset_.x, position_.y*window_size.y + offset_.y);
        }

        // Hovered over
        if (circle_shape_.getGlobalBounds().contains(mouse_pos)) {
            circle_shape_.setScale(hovered_scale_);
        } else {
            circle_shape_.setScale(1,1);
        }
    }

    bool IsClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
        return (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left &&
                circle_shape_.getGlobalBounds().contains(mouse_pos));
    }

    void DrawTo(sf::RenderWindow& window) {
        window.draw(circle_shape_);
    }

private:
    sf::CircleShape circle_shape_;
    sf::Vector2f position_;
    sf::Vector2f hovered_scale_;
    sf::Vector2f offset_;
};

} // namespace ui