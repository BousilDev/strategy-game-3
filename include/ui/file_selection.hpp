#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <tuple>

#include "constants/constants.hpp"
#include "core/utils.hpp"

namespace fs = std::filesystem;

namespace ui {

// A class for a file selector object.
class FileSelection {
public:

    FileSelection() {}

    void Initialize(const sf::Vector2f& viewSize, const std::shared_ptr<sf::Font>& font) {

        margin_ = constants::kFSmargin;
        
        default_text_.setFont(*font);
        default_text_.setCharacterSize(constants::kFileSelectionFontSize);

        // TODO: initialize all visual elements properly
        //background_ = sf::RectangleShape(sf::Vector2f(viewSize.x - viewSize.x * 2 * margin_, viewSize.y - viewSize.y * 2 * margin_));
        background_.setSize(sf::Vector2f(viewSize.x - viewSize.x * 2 * margin_, viewSize.y - viewSize.y * 2 * margin_));
        background_.setPosition(sf::Vector2f(viewSize.x * margin_, viewSize.y * margin_));
        background_.setFillColor(constants::kFSbgColor);
        background_.setOutlineColor(constants::kFSoutlineColor);
        background_.setOutlineThickness(2.f);

        visible_lines_ = background_.getSize().y / line_height_;
        
    }


    // Scan for files in the folder constants::kSavesPath.
    int ScanFiles() {
        if (constants::debug) {
            std::cout << constants::debug_prefix << "Scanning files in " << saves_folder_ << std::endl;
        }

        try {
            if (!fs::exists(saves_folder_) || !fs::is_directory(saves_folder_)) {
                std::cerr << "Not a directory: " << saves_folder_ << "\n";
                return EXIT_FAILURE;
            }
            for (const auto& file : fs::directory_iterator(saves_folder_)) {
                if (file.is_regular_file()) {
                    std::ifstream inFile(file.path().string());
                    std::string timeStr = core::DecodeTimeFromFile(inFile);
                    std::time_t timestamp = static_cast<std::time_t>(core::GetIntFromLine(inFile));

                    std::tuple<fs::path, sf::Text, time_t> textTuple = std::make_tuple(file.path(), default_text_, timestamp);
                    std::get<1>(textTuple).setString(timeStr + "   " + file.path().stem().string());
                    std::get<1>(textTuple).setOutlineColor(constants::kFSselectedTextOutline);
                    texts_.emplace_back(textTuple);

                    if (constants::debug) {
                        std::cout << constants::debug_prefix << file.path().string() << " created at: " << timeStr << std::endl;
                    }
                }
            }
        } catch (const fs::filesystem_error& error) {
            std::cerr << "Filesystem error: " << error.what() << "\n";
            return EXIT_FAILURE;
        }
        
        // FIXME: doesnt work
        // sort texts_ to reverse chronological order
        std::sort(texts_.begin(), texts_.end(), [](auto const& x, auto const& y) { return std::get<2>(x) > std::get<2>(y); });

        return 0;
    };

    void Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, const bool isLoadClicked) {

        auto windowSize = window.getSize();

        // TODO: add resizing support
        //if (event.type == sf::Event::Resized) {
        //    // update background
        //    background_.setSize(sf::Vector2f(windowSize.x - windowSize.x * 2 * margin_, windowSize.y - windowSize.y * 2 * margin_));
        //    background_.setPosition(sf::Vector2f(windowSize.x * margin_, windowSize.y * margin_));
        //
        //    //TODO: update other variables like max visible lines etc
        //    visible_lines_ = background_.getSize().y / line_height_;
        //}

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta < 0) {
                if (scroll_ + visible_lines_ < static_cast<int>(texts_.size())) scroll_++;
            } else {
                if (scroll_ > 0) scroll_--;
            }
        }

        // for deselection check
        bool clicked = false;
        bool textSelected = false;

        // check for selections and set text parameters
        const int end = std::min((int)texts_.size(), scroll_ + visible_lines_);
        for (int i = scroll_; i < end; ++i) {
            float y = windowSize.y * margin_ + (i - scroll_) * line_height_;
            std::get<1>(texts_[i]).setPosition(windowSize.x * margin_, y);
            if (!isLoadClicked &&
                event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left) {
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
    }

    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const bool& resized) {
        // update background
        if (resized) {
            background_.setSize(sf::Vector2f(window_size.x - window_size.x * 2 * margin_, window_size.y - window_size.y * 2 * margin_));
            background_.setPosition(sf::Vector2f(window_size.x * margin_, window_size.y * margin_));

            //TODO: update other variables like max visible lines etc
            visible_lines_ = background_.getSize().y / line_height_;
        }
    }

    bool IsSelected() const { return !last_clicked_path_.empty(); }

    // Reset to initial state
    void Reset() { 
        last_clicked_path_.erase();
        scroll_ = 0;
        last_clicked_index_ = -1;
        texts_.clear();
    }

    void DrawTo(sf::RenderWindow& window) {
        window.draw(background_);

        const int end = std::min((int)texts_.size(), scroll_ + visible_lines_);
        for (int i = scroll_; i < end; ++i) {
            if (i == last_clicked_index_) {
                std::get<1>(texts_[i]).setColor(constants::kFSselectedTextColor);
                std::get<1>(texts_[i]).setOutlineThickness(constants::kFileSelectionFontSize * 0.05f);
            } else {
                std::get<1>(texts_[i]).setColor(constants::kFStextColor);
                std::get<1>(texts_[i]).setOutlineThickness(0);
            }
            window.draw(std::get<1>(texts_[i]));
        }
    }

    // Returns the absolute path of the last clicked save file
    std::string& GetLastClickedPath() { return last_clicked_path_; }

private:
    const fs::path saves_folder_ = constants::kSavesPath;
    std::vector<std::tuple<fs::path, sf::Text, time_t>> texts_; // file path, text object, timestamp

    sf::RectangleShape background_;

    sf::Text default_text_;
    //const std::string no_file_selected_ = "No file has been selected!";
    std::string last_clicked_path_; // path to last clicked file in the load game screen
    int last_clicked_index_ = -1;

    int scroll_ = 0; // index of the first visible line
    int visible_lines_;
    float margin_;
    float line_height_ = constants::kFileSelectionFontSize * 1.2f;
};

} // namespace ui