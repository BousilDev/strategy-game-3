#include "core/game.hpp"
#include <iostream>

void core::Game::Initialize(const std::vector<PlayerInit>& players, unsigned int map_size) {
    // Create map with given size
    map_ = world::Map(map_size);
    // Initialize players
    for (const auto& player : players) {
        players_.push_back(std::make_unique<Player>(Player(player.name, player.deck)));
    }
    nof_players_ = players_.size();
    is_initialized_ = true;
    if (debug_) {
        std::cout << "Initialized game with " << nof_players_ << " players and map size " << map_size << "x" << map_size << std::endl;
    }
}

void core::Game::Save(std::ostream& out) const{
    // TODO
}

void core::Game::Load(std::istream& in){
    // TODO
}

bool core::Game::IsOver() {
    return nof_players_ == 1;
}
void core::Game::NextTurn() {
    turn_++;
    current_turn_ = (current_turn_ + 1) % nof_players_;
    // TODO: Update alive players
    if (debug_) {
        std::cout << "It's now " << players_[current_turn_]->GetName() << "'s turn. " << "(global turn " << turn_ << ")" << std::endl;
    }
}