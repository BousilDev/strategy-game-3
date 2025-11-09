#include "ui/map_renderer.hpp"
#include "ui/center_origin.hpp"
#include <cmath>
#include "ui/map_renderer.hpp"
#include "ui/map_renderer.hpp"

void ui::MapRenderer::Initialize(world::Map map, sf::RenderWindow& window, float tile_size) {
    
    // TODO: Bloated way to line things up, might need refactoring

    map_ = map;
    tile_size_ = tile_size;

    const size_t map_w  = map_.get_map_width();
    const auto& tile_data = map_.get_tiles();

    const float r = tile_size_;
    const float h = std::sqrt(3.f) * r;  // horizontal spacing
    const float v = 1.5f * r;            // vertical spacing

    const float hex_w = std::sqrt(3.f) * r; // width of hex
    const float hex_h = 2.f * r;            // height of hex
    const float outline = 2.f;              // outline thickness

    // Get window size
    auto window_size = window.getSize();
    const float window_w = window_size.x;
    const float window_h = window_size.y;

    tiles_.clear();
    tiles_.reserve(tile_data.size());

    // Compute centers and bounds
    std::vector<sf::Vector2f> centers;
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for (size_t i = 0; i < tile_data.size(); ++i) {
        const size_t row = i / map_w;
        const size_t col = i % map_w;

        sf::Vector2f c(
            col * h + ((row & 1) ? h * 0.5f : 0.f),
            row * v
        );

        centers.emplace_back(c);
        minX = std::min(minX, c.x);
        minY = std::min(minY, c.y);
        maxX = std::max(maxX, c.x);
        maxY = std::max(maxY, c.y);
    }

    // Visual bounds include half hex size around centers + outline
    const float map_w_px = (maxX - minX) + hex_w + 2.f * outline;
    const float map_h_px = (maxY - minY) + hex_h + 2.f * outline;

    // Top-left where the map should start
    const sf::Vector2f topLeft(
        (window_w - map_w_px) * 0.5f,
        (window_h - map_h_px) * 0.5f
    );

    // Center shift from local min center to visual top-left
    const sf::Vector2f shift(
        topLeft.x - minX + (hex_w * 0.5f) + outline,
        topLeft.y - minY + (hex_h * 0.5f) + outline
    );

    // Create shapes at final positions
    for (size_t i = 0; i < tile_data.size(); ++i) {
        const sf::Vector2f pos = centers[i] + shift;

        sf::CircleShape tile(r, 6);
        ui::centerOrigin(tile);
        tile.setPosition(pos);

        const std::string terrain = tile_data[i]->get_terrain()->get_name();
        if      (terrain == "plains")    tile.setFillColor(sf::Color(132,176,103));
        else if (terrain == "forest")    tile.setFillColor(sf::Color(93,101,50));
        else if (terrain == "mountains") tile.setFillColor(sf::Color(203,203,203));
        else if (terrain == "water")     tile.setFillColor(sf::Color(0,123,167));
        else                             tile.setFillColor(sf::Color::White);

        tile.setOutlineColor(sf::Color(0,0,0));
        tile.setOutlineThickness(outline);
        tiles_.push_back(tile);
    }
}

void ui::MapRenderer::DrawTo(sf::RenderWindow& window)  {

    for (auto v :  tiles_) {
        window.draw(v);
    }
}

// Get clicked tile. Returns nullptr when no tile is clicked!!
std::shared_ptr<world::Tile> ui::MapRenderer::GetClickedTile(sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (size_t i = 0; i < tiles_.size(); i++) {
        const auto& tile_shape = tiles_[i];
        if (tile_shape.getGlobalBounds().contains(mousePos)) {
            return map_.get_tile(i);
        }
    }
    return nullptr;
}


