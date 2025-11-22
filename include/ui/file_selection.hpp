#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

#include "constants/constants.hpp"

namespace fs = std::filesystem;

namespace ui {

// A class for a file selector object.
class FileSelection {
public:

    FileSelection() {}

    void Initialize(const std::shared_ptr<sf::Font>& font) {
        text_.setFont(*font);
        text_.setCharacterSize(constants::kFileSelectionFontSize);
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
            for (const auto& entry : fs::directory_iterator(saves_folder_)) {
                if (entry.is_regular_file()) {
                    save_files_.emplace_back(entry.path());
                    save_file_texts_.emplace_back(text_);
                    save_file_texts_[save_file_texts_.size() - 1].setString(entry.path().stem().string());
                    if (constants::debug) {
                        std::cout << constants::debug_prefix << entry.path().string() << std::endl;
                    }
                }
            }
        } catch (const fs::filesystem_error& error) {
            std::cerr << "Filesystem error: " << error.what() << "\n";
            return EXIT_FAILURE;
        }

        return 0;
    };

    void Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, const bool isLoadClicked) {

        // TODO: add resizing support

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta < 0) {
                if (scroll_ + visible_lines_ < static_cast<int>(save_files_.size())) scroll_++;
            } else {
                if (scroll_ > 0) scroll_--;
            }
        }

        const int end = std::min((int)save_files_.size(), scroll_ + visible_lines_);
        for (int i = scroll_; i < end; ++i) {
            float y = margin_ + (i - scroll_) * line_height_;
            save_file_texts_[i].setPosition(margin_, y);

            if (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left &&
                save_file_texts_[i].getGlobalBounds().contains(mousePos)) {
                last_clicked_path_ = save_files_[i].string();
                last_clicked_index_ = i;
            }

            
        }
    }

    bool IsSelected() const { return !last_clicked_path_.empty(); }

    // Reset to initial state
    void Reset() { 
        last_clicked_path_.erase();
        scroll_ = 0;
        save_files_.clear();
        save_file_texts_.clear();
        last_clicked_index_ = -1;
    }

    void DrawTo(sf::RenderWindow& window) {
        const int end = std::min((int)save_files_.size(), scroll_ + visible_lines_);
        for (int i = scroll_; i < end; ++i) {
            if (i == last_clicked_index_) {
                save_file_texts_[i].setColor(constants::kFSselectedTextColor);
            } else {
                save_file_texts_[i].setColor(constants::kFStextColor);
            }
            window.draw(save_file_texts_[i]);
        }
    }

    // Returns the absolute path of the last clicked save file
    std::string& GetLastClickedPath() { return last_clicked_path_; }

private:
    const fs::path saves_folder_ = constants::kSavesPath;
    std::vector<fs::path> save_files_;
    std::vector<sf::Text> save_file_texts_;
    sf::Text text_;
    const std::string no_file_selected_ = "No file has been selected!";
    std::string last_clicked_path_; // path to last clicked file in the load game screen
    int last_clicked_index_ = -1;

    int scroll_ = 0; // index of the first visible line
    int visible_lines_ = 4;
    float margin_ = constants::kInitWindowWidth * 0.2f;
    float line_height_ = constants::kInitWindowHeight * 0.075f;
};

} // namespace ui