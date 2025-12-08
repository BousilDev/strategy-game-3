#pragma once

/**
 * @file main_menu_renderer.hpp
 * @brief Declares the MainMenuRenderer class which handles rendering and interaction of the main menu UI.
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <assert.h>
#include <vector>
#include <filesystem>

#include "ui/selection.hpp"
#include "ui/center_origin.hpp"
#include "ui/clickable_text.hpp"
#include "ui/clickable_circle_shape.hpp"
#include "constants/constants.hpp"
#include "ui/file_selection.hpp"
#include "ui/text_input.hpp"

namespace fs = std::filesystem;

namespace ui {

/**
 * @class MainMenuRenderer
 * @brief A class for rendering and handling the main menu UI.
 */
class MainMenuRenderer {
public:

    /**
     * @brief Default constructor.
     */
    MainMenuRenderer() {}

    /**
     * @brief Initialize the MainMenuRenderer instance.
     * 
     * @param font A shared_ptr to the font used in the UI.
     * @param view_size The size of the view.
     * @return int Returns 0 if successful, 1 if there are errors.
     */
    int Initialize(const std::shared_ptr<sf::Font>& font, const sf::Vector2f& view_size);

    // Update the state of the main menu elements based on the event

    /**
     * @brief Update the MainMenuRenderer instance within the event loop.
     * 
     * @param window The render window.
     * @param mouse_pos The mouse position.
     * @param event The current event.
     * @return int Returns 0 if successful, 1 if there are errors.
     */
    int Update(const sf::RenderWindow& window, const sf::Vector2f& mouse_pos, const sf::Event& event);

    /**
     * @brief Check if the start new game button is clicked.
     * 
     * @param mouse_pos The mouse position.
     * @param event The current event.
     * @return true If the start new game button is clicked in the new game state.
     * @return false Otherwise.
     */
    bool IsStartClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
        return (current_state_ == 1 && start_new_button_.IsClicked(mouse_pos, event));
    }

    /**
     * @brief Check if the load game button is clicked and a save file is selected.
     * 
     * @param mouse_pos The mouse position.
     * @param event The current event.
     * @return true If the load game button is clicked and a save file is selected in the load game state.
     * @return false Otherwise.
     */
    bool IsLoadClicked(const sf::Vector2f& mouse_pos, const sf::Event& event) const {
        if (current_state_ == 2 && start_loaded_button_.IsClicked(mouse_pos, event) && save_file_selection_.IsSelected()) {
            if (constants::debug) {
                std::cout << constants::debug_prefix << "Load has been clicked and save file has been selected; ui::MainMenu::IsLoadClicked returns true" << std::endl;
            }
            return true;
        } else if (constants::debug && current_state_ == 2 && start_loaded_button_.IsClicked(mouse_pos, event)) {
            std::cout << constants::debug_prefix << "Load has been clicked but no save file has been selected" << std::endl;
        }
        return false;
    }

    /**
     * @brief Draw the MainMenuRenderer instance to the given window.
     * 
     * @param window The render window.
     */
    void DrawTo(sf::RenderWindow& window);

    // Getters for selected options
    int GetSelectedPlayerCount() const { return selections_[0].GetSelectedOption(); }
    int GetSelectedMapSize() const { return selections_[1].GetSelectedOption(); }
    int GetSelectedDeck() const { return selections_[2].GetSelectedOption(); }
    std::string GetGameName() const { return game_name_input_.GetInputString(); }
    std::string& GetLastClickedSavePath() { return save_file_selection_.GetLastClickedPath(); }

    // Reset the state. Used when returning to main menu from somewhere.
    void Reset() {
        current_state_ = 0;
        new_state_ = 0;
        game_name_input_.Reset();
    }

    /**
     * @brief Update the MainMenuRenderer instance outside the event loop.
     * 
     * @param window_size The size of the window.
     * @param mouse_pos The mouse position.
     * @param delta_seconds The delta time since the last update.
     */
    void UpdateOutsideEventLoop(const sf::Vector2f& window_size, const sf::Vector2f& mouse_pos, const float delta_seconds);

private:
    sf::Texture background_texture_;
    sf::RectangleShape background_rect_;

    // keep track of the current window size for different screens
    // used for resize check
    sf::Vector2f current_window_size_global_;
    sf::Vector2f current_window_size_main_;
    sf::Vector2f current_window_size_new_;
    sf::Vector2f current_window_size_load_;

    int current_state_; // 0 main menu, 1 new game, 2 load game
    int new_state_;

    ui::ClickableCircleShape back_to_main_menu_button_;

    // main menu
    sf::Text title_;
    sf::Vector2f title_pos_;
    ui::ClickableText new_game_button_;
    ui::ClickableText load_game_button_;

    // new game
    ui::ClickableText start_new_button_;
    std::vector<ui::Selection> selections_;
    ui::TextInput game_name_input_;

    // load game
    ui::ClickableText start_loaded_button_;
    ui::FileSelection save_file_selection_;
};

} // namespace ui