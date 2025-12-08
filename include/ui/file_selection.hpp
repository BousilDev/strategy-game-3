#pragma once

/**
 * @file file_selection.hpp
 * @brief Declares the FileSelection class which scans for files and lets you select them visually.
 */

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

/**
 * @class FileSelection
 * @brief A class for selecting a file via GUI from a folder.
 */
class FileSelection {
public:

    /**
     * @brief Construct an empty FileSelection instance.
     */
    FileSelection() {}

    /**
     * @brief Initialize the FileSelection instance. Must be called before using other methods.
     * 
     * @param view_size The view size.
     * @param font A shared_ptr to a font.
     */
    void Initialize(const sf::Vector2f& view_size, const std::shared_ptr<sf::Font>& font);

    // Scan for files in the folder constants::kSavesPath.
    int ScanFiles();

    /**
     * @brief Update the FileSelection instance within the event loop.
     * 
     * @param window The render window.
     * @param mousePos The mouse position.
     * @param event The current event.
     * @param isLoadClicked Whether the load button was clicked.
     */
    void Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, const bool isLoadClicked);

    /**
     * @brief Update the FileSelection instance outside the event loop.
     * 
     * @param window_size The window size.
     * @param resized Whether the window was resized.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const bool& resized);

    // Returns true if a file is selected.
    bool IsSelected() const { return !last_clicked_path_.empty(); }

    // Reset to initial state
    void Reset();

    /**
     * @brief Draw the FileSelection instance to the given window.
     * 
     * @param window The render window.
     */
    void DrawTo(sf::RenderWindow& window);

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

    // Check if content overflows visible area
    bool IsOverflow() const { return (int)texts_.size() > visible_lines_; }
    
    // Set scroll position with clamping
    void SetScroll(int s) { 
        int clamped = std::clamp(s, 0, MaxScroll());
        if (clamped != scroll_) {
            scroll_ = clamped;
            scrollbar_dirty_ = true;
        }
    }

    // Update scrollbar geometry based on current state
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