#include "ui/map_renderer.hpp"
#include "ui/center_origin.hpp"
#include <cmath>
#include "ui/map_renderer.hpp"

void ui::MapRenderer::Initialize(core::Game& game, sf::RenderWindow& window, float tile_size) {

    
    game_ = &game;
    last_turn_ = game_->GetCurrentTurn();
    map_ = game_->GetMap();
    tile_size_ = tile_size;

    const size_t map_w  = map_.get_map_width();
    // const size_t map_height = map_.get_map_height();
    const auto& tile_data = map_.get_tiles();

    const float r = tile_size_;
    const float h = std::sqrt(3.f) * r;  // horizontal spacing
    const float v = 1.5f * r;            // vertical spacing

    const float hex_w = std::sqrt(3.f) * r; // width of hex
    const float hex_h = 2.f * r;            // height of hex
    const float outline = 2.f;              // outline thickness

    // Get window size
    auto window_size = sf::Vector2f(window.getSize()) - sf::Vector2f(constants::kInitWindowWidth / 4.f, -constants::infoLayerHeight);
    const float window_w = window_size.x;
    const float window_h = window_size.y;

    tiles_.clear();
    tiles_.reserve(tile_data.size());

    // Compute centers and bounds
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    std::vector<sf::Vector2f> centers;
    centers.reserve(tile_data.size());

    for (size_t i = 0; i < tile_data.size(); ++i) {
        const size_t row = i / map_w;
        const size_t col = i % map_w;

        sf::Vector2f c(
            col * h + ((row & 1) ? h * 0.5f : 0.f),
            row * v
        );

        centers.push_back(c);
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

    // Create shapes at final positions, TEMP: add capitol buildings
    for (size_t i = 0; i < tile_data.size(); ++i) {
        const sf::Vector2f pos = centers[i] + shift;

        sf::CircleShape tile(r, 6);
        ui::centerOrigin(tile);
        tile.setPosition(pos);

        // Get terrain
        const std::string terrain = tile_data[i]->get_terrain()->get_name();
        if (terrain == "plains") tile.setFillColor(sf::Color(132,176,103));
        else if (terrain == "forest") tile.setFillColor(sf::Color(93,101,50));
        else if (terrain == "mountains") tile.setFillColor(sf::Color(203,203,203));
        else if (terrain == "water") tile.setFillColor(sf::Color(0,123,167));
        else tile.setFillColor(sf::Color::White);

        tile.setOutlineColor(sf::Color(0,0,0));
        tile.setOutlineThickness(outline);
        tiles_.push_back(tile);
    }


}

void ui::MapRenderer::DrawTo(sf::RenderWindow& window)  {


    for (size_t i = 0; i < tiles_.size(); i++) {
        window.draw(tiles_[i]);
    }

    // Draw buildings units
    for (auto v : map_.get_tiles()) {

        auto u = v-> get_unit();
            if (u) {

                sf::CircleShape unit(10);
                centerOrigin(unit);

                switch (u->GetType()) {
                    case units::UnitType::kSoldier:
                        unit.setFillColor(sf::Color(50,50,100));
                        break;
                    default:
                        unit.setFillColor(sf::Color(200,200,200));
                }

            sf::Color outline;
            auto owner_name = u->GetOwner()->GetName();
            

            if (owner_name == "Player 1") outline = constants::playerOneColor;
            else if (owner_name == "Player 2") outline = constants::playerTwoColor;
            else if (owner_name == "Player 3") outline = constants::playerThreeColor;
            else outline = constants::playerFourColor;
            
            unit.setOutlineColor(outline);
            unit.setOutlineThickness(3);
            unit.setPosition(tiles_[v->get_tile_number()].getPosition()+sf::Vector2f(15.0f, 0.0f));
            window.draw(unit);

            }

        auto b = v->get_building();
            if (b) {

                sf::RectangleShape building(sf::Vector2f(20,20));
                centerOrigin(building);
                
                switch (b->GetType()) {
                    case buildings::BuildingType::kCapital:
                        building.setFillColor(sf::Color(233,233,133));
                        break;

                    case buildings::BuildingType::kFarm:
                        building.setFillColor(sf::Color(6,233,133));
                        break;

                    default:
                        building.setFillColor(sf::Color(255,255,255));
                        break;
                }

            // Set outline colour for buildings and units
            sf::Color outline;
            auto owner_name = b->getOwner()->GetName();
            

            if (owner_name == "Player 1") outline = constants::playerOneColor;
            else if (owner_name == "Player 2") outline = constants::playerTwoColor;
            else if (owner_name == "Player 3") outline = constants::playerThreeColor;
            else outline = constants::playerFourColor;
            
            building.setOutlineColor(outline);
            building.setOutlineThickness(3);
            building.setPosition(tiles_[v->get_tile_number()].getPosition()-sf::Vector2f(15.0f, 0.0f));
            window.draw(building);

            }

    }
    
}

// Returns the tile that was clicked with Mouse 1 on the map. Returns nullptr when no tile is clicked!!
std::shared_ptr<world::Tile> ui::MapRenderer::GetClickedTile(sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (size_t i = 0; i < tiles_.size(); i++) {
        const auto& tile_shape = tiles_[i];
        if (tile_shape.getGlobalBounds().contains(mousePos)) {
            auto get_tile = map_.get_tile(i);
            if (get_tile != nullptr && get_tile->get_building() != nullptr)
                std::cout << get_tile->get_building() << std::endl;
            selected_tile_ = get_tile;
            return get_tile;
        }
    }
    selected_tile_ = nullptr;
    return nullptr;
}

// Returns nullptr if no tile is currently selected !!
std::shared_ptr<world::Tile> ui::MapRenderer::GetLastClickedTile() {
  return selected_tile_;
}

void ui::MapRenderer::PanMap(sf::RenderWindow& window) {
    
    sf::Vector2f dir(0.f, 0.f);
    float speed = constants::mapPanSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        dir.x += 1.f;

    if (dir != sf::Vector2f(0.f, 0.f))
    {
        sf::View view = window.getView();
        view.move(dir * speed);
        window.setView(view);
    }
}

void ui::MapRenderer::SetViewOnPlayer(sf::RenderWindow& window) {
    if (game_->GetCurrentTurn() > last_turn_) {
        std::shared_ptr<buildings::Building> v = game_->GetCurrentPlayer().GetCapitalBuilding();
        if (v != nullptr) {
            auto centered_tile = tiles_[v->getTile()->get_tile_number()];
            sf::View view = window.getView();
            view.setCenter(centered_tile.getPosition());
            window.setView(view);
        }
        last_turn_ += 1;
    }
}