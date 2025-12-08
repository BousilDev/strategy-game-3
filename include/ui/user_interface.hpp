#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ui/main_menu_renderer.hpp"
#include "ui/map_renderer.hpp"
#include "ui/info_layer_renderer.hpp"
#include "constants/constants.hpp"
#include "core/game.hpp"

namespace ui {


class UserInterface {
public:

    UserInterface() {};

    int Initialize(core::Game& game);

    //FIXME: using this in the event handling loop in main causes a weird bug
    // Update mousepos, poll next event and handle it
    //bool PollAndHandleEvent(bool start);

    // Update mouse position and poll events. Returns true if event found.
    bool PollEvent();

    // TODO: Temp event getter thing for map rendering soz
    sf::Event& GetEvent() { return event_; };

    // Handle current event
    void HandleEvent(bool start);

    std::shared_ptr<world::Tile> GetLastClickedTile();

    // check if play button is clicked
    bool IsStartClicked();

    // check if load button is clicked
    bool IsLoadClicked();

    bool IsKeyPressed();

    void InitializeMapRenderer(core::Game& game) {
        map_renderer_.Initialize(game, window_);
    }

    // clears the window and draws sprites
    void DrawAndDisplay(bool start);

    sf::RenderWindow& GetWindow() { return window_; }

    int GetSelectedPlayerCount() const { return main_menu_renderer_.GetSelectedPlayerCount(); }
    int GetSelectedMapSize() const { return main_menu_renderer_.GetSelectedMapSize(); }
    int GetSelectedDeck() const { return main_menu_renderer_.GetSelectedDeck(); }
    std::string GetGameName() const { return main_menu_renderer_.GetGameName(); }

    std::string& GetLastClickedSavePath() { return main_menu_renderer_.GetLastClickedSavePath(); }

    // Outside of event loop
    void UpdateOutsideEventLoop(const bool start, const float delta_seconds);

    // Reset main menu. Used when returning to main menu.
    void ResetMainMenu() { main_menu_renderer_.Reset(); }

    // Return true if view is not centered.
    bool IsViewNotCentered() const {
        return (view_.getCenter().x != window_.getSize().x || view_.getCenter().y != window_.getSize().y);
    }

    // Center the view
    void CenterView() {
        view_.setCenter(window_.getSize().x * 0.5f, window_.getSize().y * 0.5f);
        window_.setView(view_);
    }

private:
    sf::RenderWindow window_;
    sf::View view_;
    sf::Vector2f view_size_;
    sf::Vector2f mouse_pos_;

    //zooming variables
    float zoom_ = 1.0f;
    static constexpr float kMinZoom_ = 0.5f;
    static constexpr float kMaxZoom_ = 2.5f;
    
    // TODO: fix event handling stuff
    sf::Event event_;

    std::shared_ptr<sf::Font> font_ = std::make_shared<sf::Font>();
    ui::MainMenuRenderer main_menu_renderer_;
    ui::MapRenderer map_renderer_;
    ui::InfoLayerRenderer info_layer_renderer_;
};

} // namespace ui