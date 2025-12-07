#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <tuple>

#include <algorithm>
#include <sstream>
#include <cmath>

#include "constants/constants.hpp"
#include "core/utils.hpp"

namespace fs = std::filesystem;

namespace ui {

// A class for a file selector object.
class FileSelection {
public:

    FileSelection() {}

    void Initialize(const sf::Vector2f& viewSize, const std::shared_ptr<sf::Font>& font) {
        
        default_text_.setFont(*font);
        default_text_.setCharacterSize(constants::kFileSelectionFontSize);

        background_.setSize(sf::Vector2f(viewSize.x - viewSize.x * 2 * margin_, viewSize.y - viewSize.y * 2 * margin_));
        background_.setPosition(sf::Vector2f(viewSize.x * margin_, viewSize.y * margin_));
        background_.setFillColor(constants::kFSbgColor);
        background_.setOutlineColor(constants::kFSoutlineColor);
        background_.setOutlineThickness(2.f);

        visible_lines_ = background_.getSize().y / line_height_;

        // Scrollbar
        //scrollbar_track_.setOutlineThickness(0.f);
        //scrollbar_thumb_.setOutlineThickness(0.f);
        scrollbar_track_.setFillColor(constants::kScrollbarTrackColor);
        scrollbar_thumb_.setFillColor(constants::kScrollbarThumbColor);

        UpdateScrollbarGeometry();
    }


    // Scan for files in the folder constants::kSavesPath.
    int ScanFiles() {
        if (constants::debug) { core::PrintTestMsg("Scanning files in ", saves_folder_); }
        try {
            if (!fs::exists(saves_folder_) || !fs::is_directory(saves_folder_)) {
                std::cerr << "Not a directory: " << saves_folder_ << "\n";
                return EXIT_FAILURE;
            }
            for (const auto& file : fs::directory_iterator(saves_folder_)) {
                if (file.is_regular_file()) {
                    std::ifstream inFile(file.path().string());
                    std::time_t timestamp = static_cast<std::time_t>(core::GetIntFromLine(inFile));
                    std::istringstream tmp(std::to_string(static_cast<long long>(timestamp)));
                    std::string time_str = core::DecodeTimeFromFile(tmp);

                    std::string label = time_str + "   " + file.path().stem().string();
                    sf::Text text = default_text_;
                    text.setString(label);
                    text.setOutlineColor(constants::kFSselectedTextOutline);
                    texts_.emplace_back(file.path(), std::move(text), timestamp);

                    if (constants::debug) {
                        core::PrintTestMsg(file.path().string(), " created at: ", time_str, "   ", timestamp);
                    }
                }
            }
        } catch (const fs::filesystem_error& error) {
            std::cerr << "Filesystem error: " << error.what() << "\n";
            return EXIT_FAILURE;
        }
        
        // sort texts_ to reverse chronological order
        std::stable_sort(texts_.begin(), texts_.end(), [](auto const& x, auto const& y) { return std::get<2>(x) > std::get<2>(y); });

        // Clamp scroll and refresh scrollbar
        SetScroll(std::min(scroll_, MaxScroll()));
        scrollbar_dirty_ = true;

        return 0;
    };

    void Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, const bool isLoadClicked) {

        auto windowSize = window.getSize();

        const bool overflow = IsOverflow();
        const bool mouseOverBackground = background_.getGlobalBounds().contains(mousePos);

        // Mouse wheel scrolling when cursor is over the list area
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (mouseOverBackground && overflow) {
                int steps = (event.mouseWheelScroll.delta < 0) ? 1 : -1;
                SetScroll(scroll_ + steps);
            }
        }

        // Start drag or page on track click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (overflow) {
                if (scrollbar_thumb_.getGlobalBounds().contains(mousePos)) {
                    scrollbar_dragging_ = true;
                    scrollbar_grab_offset_y_ = mousePos.y - scrollbar_thumb_.getPosition().y;
                } else if (scrollbar_track_.getGlobalBounds().contains(mousePos)) {
                    // Click on track pages up/down
                    float thumbTop = scrollbar_thumb_.getPosition().y;
                    float thumbBottom = thumbTop + scrollbar_thumb_.getSize().y;
                    if (mousePos.y < thumbTop) {
                        SetScroll(scroll_ - visible_lines_); // page up
                    } else if (mousePos.y > thumbBottom) {
                        SetScroll(scroll_ + visible_lines_); // page down
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            scrollbar_dragging_ = false;
        }

        // Dragging the thumb moves scroll
        if (event.type == sf::Event::MouseMoved && scrollbar_dragging_ && overflow) {
            const float trackY = scrollbar_track_.getPosition().y;
            const float trackH = scrollbar_track_.getSize().y;
            float thumbH = scrollbar_thumb_.getSize().y;

            float maxThumbTravel = std::max(0.f, trackH - thumbH);
            float targetTop = std::clamp(mousePos.y - scrollbar_grab_offset_y_, trackY, trackY + maxThumbTravel);

            float t = (maxThumbTravel > 0.f) ? (targetTop - trackY) / maxThumbTravel : 0.f;
            int newScroll = (int)std::round(t * MaxScroll());
            SetScroll(newScroll);
        }

        // for deselection check
        bool clicked = false;
        bool textSelected = false;

        // Update text positions, check for selections and set text parameters
        const sf::Vector2f bg_pos = background_.getPosition();
        const int end = std::min((int)texts_.size(), scroll_ + visible_lines_);
        for (int i = scroll_; i < end; ++i) {
            float y = bg_pos.y + content_padding_ + (i - scroll_) * line_height_;
            float x = bg_pos.x + content_padding_;
            std::get<1>(texts_[i]).setPosition(x, y);
            if (!isLoadClicked &&
                event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left) {

                if (overflow && (scrollbar_thumb_.getGlobalBounds().contains(mousePos) ||
                                 scrollbar_track_.getGlobalBounds().contains(mousePos))) {
                    continue;
                }

                if (std::get<1>(texts_[i]).getGlobalBounds().contains(mousePos)) {
                    last_clicked_path_ = std::get<0>(texts_[i]).string();
                    last_clicked_index_ = i;
                    textSelected = true;
                } else {
                    clicked = true;
                }
            }
        }

        if (!textSelected && clicked) {
            last_clicked_path_.erase();
            last_clicked_index_ = -1;
        }

        if (scrollbar_dirty_) {
            UpdateScrollbarGeometry();
            scrollbar_dirty_ = false;
        }
    }

    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const bool& resized) {
        // update background
        if (resized) {
            background_.setSize(sf::Vector2f(window_size.x - window_size.x * 2 * margin_, window_size.y - window_size.y * 2 * margin_));
            background_.setPosition(sf::Vector2f(window_size.x * margin_, window_size.y * margin_));
            visible_lines_ = background_.getSize().y / line_height_;
            SetScroll(std::min(scroll_, MaxScroll())); // clamp within range
            scrollbar_dirty_ = true;
        }
    }

    bool IsSelected() const { return !last_clicked_path_.empty(); }

    // Reset to initial state
    void Reset() { 
        last_clicked_path_.erase();
        scroll_ = 0;
        last_clicked_index_ = -1;
        texts_.clear();
        scrollbar_dragging_ = false;
        UpdateScrollbarGeometry();
    }

    void DrawTo(sf::RenderWindow& window) {
        window.draw(background_);

        const int end = std::min((int)texts_.size(), scroll_ + visible_lines_);
        for (int i = scroll_; i < end; ++i) {
            auto& text = std::get<1>(texts_[i]);
            text.setFillColor(i == last_clicked_index_ ? constants::kFSselectedTextColor : constants::kFStextColor);
            text.setOutlineThickness(i == last_clicked_index_ ? constants::kFileSelectionFontSize * 0.05f : 0.f);
            window.draw(std::get<1>(texts_[i]));
        }
        
        if (IsOverflow()) {
            if (scrollbar_dirty_) {
                UpdateScrollbarGeometry();
                scrollbar_dirty_ = false;
            }
            window.draw(scrollbar_track_);
            window.draw(scrollbar_thumb_);
        }
    }

    // Returns the absolute path of the last clicked save file
    std::string& GetLastClickedPath() { return last_clicked_path_; }

private:
    const fs::path saves_folder_ = constants::kSavesPath;
    std::vector<std::tuple<fs::path, sf::Text, time_t>> texts_; // file path, text object, timestamp

    sf::RectangleShape background_;

    sf::Text default_text_;
    std::string last_clicked_path_; // path to last clicked file in the load game screen
    int last_clicked_index_ = -1;

    int scroll_ = 0; // index of the first visible line
    int visible_lines_;
    float margin_ = constants::kFSmargin;
    float line_height_ = constants::kFileSelectionFontSize * 1.2f;
    float content_padding_ = constants::kFScontentPadding;

    // Scrollbar
    sf::RectangleShape scrollbar_track_;
    sf::RectangleShape scrollbar_thumb_;
    bool scrollbar_dragging_ = false;
    float scrollbar_grab_offset_y_ = 0.f;
    float scrollbar_padding_ = constants::kScrollbarPadding;  // gap from right edge of background_
    float scrollbar_width_   = constants::kScrollbarWidth;  // track width
    float min_thumb_height_   = constants::kMinThumbHeight; // minimum thumb height
    bool scrollbar_dirty_ = true; // flag for checking for changes

    // Scrollbar helper functions
    int MaxScroll() const { return std::max(0, (int)texts_.size() - visible_lines_); }
    bool IsOverflow() const { return (int)texts_.size() > visible_lines_; }
    
    void SetScroll(int s) { 
        int clamped = std::clamp(s, 0, MaxScroll());
        if (clamped != scroll_) {
            scroll_ = clamped;
            scrollbar_dirty_ = true;
        }
    }

    void UpdateScrollbarGeometry() {
        const auto bgPos  = background_.getPosition();
        const auto bgSize = background_.getSize();

        const float trackH = bgSize.y;
        const float trackX = bgPos.x + bgSize.x - scrollbar_padding_ - scrollbar_width_;
        const float trackY = bgPos.y;

        scrollbar_track_.setPosition({trackX, trackY});
        scrollbar_track_.setSize({scrollbar_width_, trackH});

        if (!IsOverflow()) {
            // Hide if no overflow
            scrollbar_track_.setFillColor(sf::Color::Transparent);
            scrollbar_thumb_.setSize({0.f, 0.f});
            return;
        } else {
            scrollbar_track_.setFillColor(constants::kScrollbarTrackColor);
        }

        // Thumb size based on visible/total ratio
        const int totalLines = (int)texts_.size();
        const float visibleH = bgSize.y;
        const float contentH = totalLines * line_height_;
        const float ratio = std::clamp(visibleH / contentH, 0.f, 1.f);

        float thumbH = std::max(min_thumb_height_, trackH * ratio);
        thumbH = std::min(thumbH, trackH);

        // Map scroll_ to thumb position
        const int maxScroll = MaxScroll();
        float maxThumbTravel = std::max(0.f, trackH - thumbH);
        float t = (maxScroll > 0) ? float(scroll_) / float(maxScroll) : 0.f;
        float thumbY = trackY + t * maxThumbTravel;

        scrollbar_thumb_.setPosition({trackX, thumbY});
        scrollbar_thumb_.setSize({scrollbar_width_, thumbH});
    }
};

} // namespace ui