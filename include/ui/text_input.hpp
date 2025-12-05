#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace ui {

class TextInput {
public:

    TextInput() {}

    TextInput(const sf::Vector2f& view_size, const std::string& prefix, const sf::Font& font, const sf::Vector2f& position,
              const sf::Vector2f& offset = sf::Vector2f(0, 0), unsigned int character_size = 30U)
              : prefix_(prefix), position_(position), offset_(offset) {
        text_ = sf::Text(prefix_, font, character_size);
        text_.setPosition(position_.x*view_size.x + offset_.x, position_.y*view_size.y + offset_.y);
    }

    void UpdateEvent(const sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') {
                input_string_ = input_string_.substr(0, input_string_.size() - 1);
            } else if (event.text.unicode == '\r' || event.text.unicode == '\n' || event.text.unicode == '\t') {
                // ignore enter key
            } 
            else {
                input_string_ += event.text.unicode;
            }
        }
    }

    void UpdatePosition(const sf::Vector2f& window_size) {
        text_.setPosition(position_.x*window_size.x + offset_.x, position_.y*window_size.y + offset_.y);
    }

    void DrawTo(sf::RenderWindow& window) {
        text_.setString(prefix_ + input_string_);
        window.draw(text_);
    }

    std::string GetInputString() const { return input_string_; }

private:
    sf::Text text_;
    std::string input_string_;
    std::string prefix_;
    sf::Vector2f position_;
    sf::Vector2f offset_;
};

} // namespace ui