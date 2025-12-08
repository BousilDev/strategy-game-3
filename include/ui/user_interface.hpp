#pragma once

/**
 * @file user_interface.hpp
 * @brief Declares the UserInterface class which manages the overall user interface including main menu, map rendering, and info layer.
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ui/main_menu_renderer.hpp"
#include "ui/map_renderer.hpp"
#include "ui/info_layer_renderer.hpp"
#include "constants/constants.hpp"
#include "core/game.hpp"

namespace ui {

/**
 * @class UserInterface
 * @brief Manages the overall user interface including main menu, map rendering, and info layer.
 */
class UserInterface {
public:

    /**
     * @brief Default constructor.
     */
    UserInterface() {};

    /**
     * @brief Initialize the UserInterface instance.
     * 
     * @param game A reference to the core::Game instance.
     * @return int Returns 0 if successful, 1 if there are errors.
     */
    int Initialize(core::Game& game);

    /**
     * @brief Poll events and update mouse position.
     * 
     * @return true If an event was polled.
     * @return false Otherwise.
     */
    bool PollEvent();

    /**
     * @brief Get the current SFML event.
     * 
     * @return sf::Event& A reference to the current event.
     */
    sf::Event& GetEvent() { return event_; };

    // Handle current event
    void HandleEvent(bool start, std::shared_ptr<bool> game_ended);

    /**
     * @brief Get the tile that was last clicked.
     * 
     * @return std::shared_ptr<world::Tile> A shared pointer to the last clicked tile.
     */
    std::shared_ptr<world::Tile> GetLastClickedTile();

    /**
     * @brief Check if the play button is clicked.
     * 
     * @return true If the play button is clicked.
     * @return false Otherwise.
     */
    bool IsStartClicked();

    /**
     * @brief Check if the load button is clicked.
     * 
     * @return true If the load button is clicked.
     * @return false Otherwise.
     */
    bool IsLoadClicked();

    /**
     * @brief Initialize the map renderer.
     * 
     * @param game A reference to the core::Game instance.
     */
    void InitializeMapRenderer(core::Game& game) {
        map_renderer_.Initialize(game, window_);
    }

    // clears the window and draws sprites
    void DrawAndDisplay(bool start, bool game_ended);

    /**
     * @brief Get the SFML render window.
     * 
     * @return sf::RenderWindow& A reference to the render window.
     */
    sf::RenderWindow& GetWindow() { return window_; }

    // Getters for selected options in main menu
    int GetSelectedPlayerCount() const { return main_menu_renderer_.GetSelectedPlayerCount(); }
    int GetSelectedMapSize() const { return main_menu_renderer_.GetSelectedMapSize(); }
    int GetSelectedDeck() const { return main_menu_renderer_.GetSelectedDeck(); }
    std::string GetGameName() const { return main_menu_renderer_.GetGameName(); }
    std::string& GetLastClickedSavePath() { return main_menu_renderer_.GetLastClickedSavePath(); }

    /**
     * @brief Update the UserInterface instance outside the event loop.
     * 
     * @param start Whether the game has started (true) or is in the main menu (false).
     * @param delta_seconds The time elapsed since the last update in seconds.
     */
    void UpdateOutsideEventLoop(const bool start, const float delta_seconds);

    /** 
     * @brief Reset the main menu to its initial state.
     */
    void ResetMainMenu() { main_menu_renderer_.Reset(); }

    /**
     * @brief Check if the view is not centered.
     * 
     * @return true If the view is not centered.
     * @return false Otherwise.
     */
    bool IsViewNotCentered() const {
        return (view_.getCenter().x != window_.getSize().x || view_.getCenter().y != window_.getSize().y);
    }

    /**
     * @brief Center the view.
     */
    void CenterView() {
        view_.setCenter(window_.getSize().x * 0.5f, window_.getSize().y * 0.5f);
        window_.setView(view_);
    }

private:
    sf::RenderWindow window_;
    sf::View view_;
    sf::Vector2f view_size_;
    sf::Vector2f mouse_pos_;
    sf::Event event_;
    std::shared_ptr<sf::Font> font_ = std::make_shared<sf::Font>();
    ui::MainMenuRenderer main_menu_renderer_;
    ui::MapRenderer map_renderer_;
    ui::InfoLayerRenderer info_layer_renderer_;
};

} // namespace ui