#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

namespace ui {

class TextInput {
public:
    TextInput() = default;

    TextInput(const sf::Vector2f& view_size, const std::string& prefix, const sf::Font& font, const sf::Vector2f& position,
              const sf::Vector2f& offset = sf::Vector2f(0, 0), unsigned int character_size = 30U)
              : prefix_(prefix), position_(position), offset_(offset) {
        text_.setFont(font);
        text_.setCharacterSize(character_size);
        text_.setString(prefix_);
        text_.setPosition(position_.x*view_size.x + offset_.x, position_.y*view_size.y + offset_.y);

        caret_.setFillColor(sf::Color::White);
        caret_.setSize({1.f, static_cast<float>(character_size)});
        caret_.setPosition(text_.getPosition());
    }

    void UpdateEvent(const sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            const sf::Uint32 u = event.text.unicode;

            // Backspace
            if (u == 8) {
                if (buffer_.getSize() > 0) {
                    buffer_.erase(buffer_.getSize() - 1);
                    dirty_ = true;
                }
                return;
            }

            if (!(std::isalnum(u) || u==' ' || u=='_' || u=='-')) return;

            // Enforce max length
            if (buffer_.getSize() >= max_length_) {
                return;
            }

            buffer_.insert(buffer_.getSize(), u);
            dirty_ = true;
        }
        else if (event.type == sf::Event::KeyPressed) {
            // Clear (Escape)
            if (event.key.code == sf::Keyboard::Escape) {
                Reset();
            }
        }
    }

    void UpdatePosition(const sf::Vector2f& window_size) {
        text_.setPosition(position_.x * window_size.x + offset_.x,
                          position_.y * window_size.y + offset_.y);
        dirty_ = true;
    }

    // Call every frame with delta time to update the caret blink
    void UpdateOutsideEventLoop(float dt) {
        caret_time_ += dt;
        if (caret_time_ >= caret_blink_period_) {
            caret_time_ = 0.f;
            caret_visible_ = !caret_visible_;
        }
        if (dirty_) {
            text_.setString(prefix_ + buffer_);
            UpdateCaretPosition();
            dirty_ = false;
        }
    }

    void DrawTo(sf::RenderWindow& window) {
        window.draw(text_);
        if (caret_visible_) {
            window.draw(caret_);
        }
    }

    std::string GetInputString() const {
        sf::String s = buffer_;
        std::string out;
        out.reserve(s.getSize());
        for (std::size_t i = 0; i < s.getSize(); ++i) {
            sf::Uint32 u = s[i];
            if (u <= 0xFF) out.push_back(static_cast<char>(u));
            else {
                // drop non-Latin-1 chars in this simple conversion
            }
        }
        return out;
    }

    const sf::String& GetBuffer() const { return buffer_; }

    void Reset() {
        buffer_.clear();
        dirty_ = true;
    }

private:
    void UpdateCaretPosition() {
        const auto bounds = text_.getLocalBounds();
        const float x = text_.getPosition().x + bounds.left + bounds.width;
        const float y = text_.getPosition().y;
        caret_.setPosition({x + 2.f, y + 5.f});
    }

    sf::Text text_;
    sf::RectangleShape caret_;

    sf::String buffer_;
    std::string prefix_;

    sf::Vector2f position_{0.f, 0.f};
    sf::Vector2f offset_{0.f, 0.f};

    std::size_t max_length_ = constants::TextInputMaxLength;
    //std::function<bool(sf::Uint32)> char_filter_;

    bool dirty_ = true;

    // Caret blink state
    bool caret_visible_ = true;
    float caret_time_ = constants::TextInputCaretTime;
    float caret_blink_period_ = constants::TextInputCaretBlinkPeriod;
};

} // namespace ui
