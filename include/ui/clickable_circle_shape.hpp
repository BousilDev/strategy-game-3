#pragma once

/**
 * @file clickable_circle_shape.hpp
 * @brief Declares the ClickableCircleShape class which is a wrapper for sf::CircleShape that handles clicks and hover effects.
 */

#include <SFML/Graphics.hpp>

#include "constants/constants.hpp"
#include "ui/center_origin.hpp"

namespace ui {

/**
 * @class ClickableCircleShape
 * @brief A wrapper for sf::CircleShape that handles clicks and hover effects.
 */
class ClickableCircleShape {
public:

    /**
     * @brief Default constructor.
     */
    ClickableCircleShape() {}

    /**
     * @brief Construct a ClickableCircleShape instance.
     * 
     * @param view_size The size of the view.
     * @param radius The radius of the circle.
     * @param point_count The number of points of the circle.
     * @param position The relative position (0.0 to 1.0) of the circle in the window.
     * @param angle The rotation angle of the circle in degrees. Default is 0.
     * @param offset The pixel offset from the relative position. Default is (0,0).
     */
    ClickableCircleShape(const sf::Vector2f& view_size, float radius, std::size_t point_count, sf::Vector2f position,
                         float angle = 0, sf::Vector2f offset = sf::Vector2f(0,0))
                        : position_(position), offset_(offset) {
        circle_shape_ = sf::CircleShape(radius, point_count);
        centerOrigin(circle_shape_);
        circle_shape_.setRotation(angle);
        circle_shape_.setPosition(position_.x*view_size.x + offset_.x, position.y*view_size.y + offset_.y);
        circle_shape_.setFillColor(normal_color_);
        circle_shape_.setOutlineColor(outline_color_);
        circle_shape_.setOutlineThickness(outline_size_);
    }

    /**
     * @brief Update the ClickableCircleShape instance outside the event loop.
     * 
     * @param window_size The size of the window.
     * @param mouse_pos The mouse position.
     * @param resized Whether the window was resized.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const sf::Vector2f& mouse_pos, const bool& resized) {
        // Resized
        if (resized) {
            circle_shape_.setPosition(position_.x*window_size.x + offset_.x, position_.y*window_size.y + offset_.y);
        }

        // Hovered over
        if (circle_shape_.getGlobalBounds().contains(mouse_pos)) {
            circle_shape_.setScale(hovered_scale_);
            circle_shape_.setFillColor(hovered_color_);
        } else {
            circle_shape_.setScale(1, 1);
            circle_shape_.setFillColor(normal_color_);
        }
    }

    /**
     * @brief Check if the circle shape is clicked.
     * 
     * @param mouse_pos The mouse position.
     * @param event The current event.
     * @return true If the circle shape is clicked.
     * @return false Otherwise.
     */
    bool IsClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
        return (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left &&
                circle_shape_.getGlobalBounds().contains(mouse_pos));
    }

    /**
     * @brief Draw the ClickableCircleShape instance to the given window.
     * 
     * @param window The render window.
     */
    void DrawTo(sf::RenderWindow& window) {
        window.draw(circle_shape_);
    }

private:
    sf::CircleShape circle_shape_;
    sf::Vector2f position_;
    sf::Vector2f offset_;
    sf::Vector2f hovered_scale_ = constants::kClickableCircleShapeHoveredScale;
    sf::Color normal_color_ = constants::kClickableNormalColor;
    sf::Color hovered_color_ = constants::kClickableHoveredColor;
    sf::Color outline_color_ = constants::kClickableOutlineColor;
    float outline_size_ = constants::kClickableOutlineSize;
};

} // namespace ui