#pragma once

/**
 * @file cool_title.hpp
 * @brief Declares the CoolTitle class which handles animated title rendering.
 */

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <algorithm>

#include "constants/constants.hpp"

namespace fs = std::filesystem;

namespace ui {

/**
 * @class CoolTitle
 * @brief A class for rendering a cool animated title using frame images.
 * 
 * The logic in this class is mostly AI generated.
 */
class CoolTitle {
public:

    /**
     * @brief Default constructor.
     */
    CoolTitle() = default;

    /**
     * @brief Initialize the CoolTitle instance.
     * 
     * @param view_size The size of the view.
     * @param position The relative position (0.0 to 1.0) in the window.
     * @param offset The pixel offset from the relative position.
     */
    void Initialize(const sf::Vector2f& view_size, const sf::Vector2f& position, const sf::Vector2f& offset) {

        position_ = position;
        offset_ = offset;

        // 1) Enumerate frames
        frame_files_ = findFrameFiles(frames_path);

        // 2) Load textures and create sprites
        textures_.resize(frame_files_.size());
        sprites_.resize(frame_files_.size());

        for (size_t i = 0; i < frame_files_.size(); ++i) {
            if (!textures_[i].loadFromFile(frame_files_[i].string())) {
                throw std::runtime_error("Failed to load " + frame_files_[i].string());
            }
            sprites_[i].setTexture(textures_[i]);
            sprites_[i].setPosition(view_size.x*position_.x + offset_.x, view_size.y*position_.y + offset_.y);
        }

        // 3) Read per-frame delays (seconds)
        delays_ = readDelaysCs(delays_file, frame_files_.size());
    }

    /**
     * @brief Update the CoolTitle instance outside the event loop.
     * 
     * @param window_size The window size.
     * @param delta_seconds The delta time since the last update.
     * @param resized Whether the window was resized.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const float delta_seconds, const bool resized) {

        if (resized) {
            for (auto& sprite : sprites_) {
                sprite.setPosition(window_size.x*position_.x + offset_.x, window_size.y*position_.y + offset_.y);
            }
        }

        // advance frames accounting for varying per-frame delays
        accumulator_ += delta_seconds;
        while (accumulator_ >= delays_[idx_]) {
            accumulator_ -= delays_[idx_];
            idx_ = (idx_ + 1) % sprites_.size();
        }
    }

    /**
     * @brief Draw the CoolTitle instance to the given window.
     * 
     * @param window The render window.
     */
    void DrawTo(sf::RenderWindow& window) {
        window.draw(sprites_[idx_]);
    }

private:
    sf::Vector2f position_;
    sf::Vector2f offset_;
    float accumulator_ = 0.f;
    size_t idx_ = 0;
    std::vector<fs::path> frame_files_;
    std::vector<sf::Texture> textures_;
    std::vector<sf::Sprite> sprites_;
    std::vector<float> delays_;
    const fs::path frames_path = constants::kFramesPath;
    const fs::path delays_file = constants::kDelaysFile;


    // Utility: find frame_###.png files in a directory
    std::vector<fs::path> findFrameFiles(const fs::path& dir) {
        std::vector<fs::path> files;
        std::regex pattern(R"(frame_(\d{3})\.png)", std::regex::icase);

        if (!fs::exists(dir) || !fs::is_directory(dir)) {
            throw std::runtime_error("Frames directory not found: " + dir.string());
        }

        for (auto& entry : fs::directory_iterator(dir)) {
            if (!entry.is_regular_file()) continue;
            const auto name = entry.path().filename().string();
            if (std::regex_match(name, pattern)) {
                files.push_back(entry.path());
            }
        }

        std::sort(files.begin(), files.end(), [](const fs::path& a, const fs::path& b) {
            static std::regex re(R"(frame_(\d+)\.png)", std::regex::icase);
            std::smatch ma, mb;
            std::string sa = a.filename().string();
            std::string sb = b.filename().string();
            int ia = 0, ib = 0;
            if (std::regex_match(sa, ma, re) && ma.size() > 1) ia = std::stoi(ma[1].str());
            if (std::regex_match(sb, mb, re) && mb.size() > 1) ib = std::stoi(mb[1].str());
            return ia < ib;
        });

        if (files.empty()) {
            throw std::runtime_error("No frame_###.png files found in " + dir.string());
        }
        return files;
    }

    // Utility: read delays (centiseconds) from a file, one integer per line
    std::vector<float> readDelaysCs(const fs::path& file, size_t count) {
        std::vector<float> delays;
        std::ifstream ifs(file);
        if (!ifs) {
            std::cerr << "Warning: could not open " << file << ". Using default 10cs (0.10s) per frame.\n";
            return std::vector<float>(count, 0.10f);
        }
        int cs = 10;
        while (ifs >> cs) {
            // Convert to seconds; clamp zero to a small positive
            if (cs <= 0) cs = 1;
            delays.push_back(cs / 100.f);
        }
        if (delays.size() != count) {
            std::cerr << "Warning: delays count (" << delays.size()
                      << ") != frames count (" << count << ")."
                      << " Resizing with repeat/trim.\n";
            // Adjust size: repeat last or trim
            if (delays.size() < count) {
                if (!delays.empty()) {
                    delays.resize(count, delays.back());
                } else {
                    delays.assign(count, 0.10f);
                }
            } else {
                delays.resize(count);
            }
        }
        return delays;
    }
};

} // namespace ui