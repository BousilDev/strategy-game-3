#pragma once

#include "core/game.hpp"
#include <SFML/Graphics.hpp>

namespace ui {
 
template <typename T>
struct DrawItem {
    std::string description;
    T value;
};

class InfoLayerRenderer {
public:
    InfoLayerRenderer() {};
    /**
     * @brief Initialize the info layer renderer.
     *
     * Prepare internal resources required to render the info layer (e.g. text
     * objects, textures, vertex buffers) using the provided game context and font.
     * This must be called before any update/draw calls on the renderer.
     *
     * @param game
     *   Reference to the core::Game instance providing access to game state and
     *   services required by the renderer. The caller must ensure that 'game'
     *   remains valid for the lifetime of the renderer.
     *
     * @param font
     *   Shared pointer to an sf::Font used for all text rendering in the info
     *   layer. The pointer must be non-null; the renderer keeps a copy of the
     *   shared_ptr, so the font will remain alive while the renderer holds it.
     *
     * @post Internal rendering resources are allocated and the renderer is ready
     *       to accept update and draw calls.
     *
     * @throws std::invalid_argument if 'font' is null.
     */
    void Initialize(core::Game& game, const std::shared_ptr<sf::Font> font);

    /**
     * @brief Draws the info-layer UI contents to the given render window.
     *
     * Draws UI elements that belong to the info-layer (Info bar, cards, tile info view). 
     * This routine issues the necessary draw calls to the provided sf::RenderWindow; it does not call
     * window.display() and therefore should be invoked as part of the active frame's render sequence.
     *
     * @param window Reference to the sf::RenderWindow to draw the info layer into.
     */
    void DrawTo(sf::RenderWindow& window);
    
    
    /**
     * @brief Update the info-layer renderer state and visuals.
     *
     * Uses the provided render window, current mouse position and SFML event to
     * determine which UI elements are selected.
     *
     * @param window Reference to the active render window
     * @param mousePos Current mouse position
     * @param event Recent SFML event to be handled by the info layer; irrelevant
     *              events are ignored.
     * @param tile_pointer Shared pointer to the tile currently under the cursor
     *                     (may be null).
     */
    void Update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, std::shared_ptr<world::Tile> tile_pointer);

    void SetSelectedTile(std::shared_ptr<world::Tile> tile) { selected_tile_ = tile; }

private: 
    core::Game* game_ = nullptr;
    float tile_size_;
    std::shared_ptr<sf::Font> font_;
    std::vector<DrawItem<std::string>> items_;
    std::vector<DrawItem<std::string>> resource_items_;
    std::shared_ptr<world::Tile> selected_tile_ = nullptr;
    std::shared_ptr<cards::Card> selected_card_ = nullptr;
    bool initial_draw_ = true;

    sf::RectangleShape background_;
    sf::RectangleShape nextTurnButton_;
    sf::Text nextTurnText_;
    sf::RectangleShape tileInfoBackground_;
    sf::Text tileInfoText_;
    sf::RectangleShape infoBackground_;
    sf::Text infoText_;
    sf::RectangleShape cardBackground_;
    std::vector<sf::RectangleShape> cardBackgrounds_;

    int parts_ = 2;
    float inc_ = (constants::infoLayerHeight - float(constants::infoLayerTextSize)) / float(parts_);

    void UpdateDrawItems();
    sf::Vector2f GetFixedPosition(sf::RenderWindow& window, const sf::Vector2f& relativePos, bool absolute = true);
    int DrawItemAtLocation(sf::RenderWindow& window, DrawItem<std::string> item, sf::Vector2f location);
    void DrawItems(sf::RenderWindow& window, std::vector<DrawItem<std::string>> items, int& textLen);
    void DrawCardAtLocation(sf::RenderWindow& window, std::shared_ptr<cards::Card> card, sf::Vector2f location, int index);
    void DrawCards(sf::RenderWindow& window);
    std::string GetTileInfoString();
    std::string GetCardInfoString(std::shared_ptr<cards::Card> card, float width, bool inCard);

    bool isNextTurnClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos);
    std::shared_ptr<cards::Card> CardClicked(const sf::RenderWindow& window, const sf::Vector2f& mousePos);
};

} // namespace ui