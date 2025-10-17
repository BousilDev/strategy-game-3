#include "core/game.hpp"
#include <iostream>

void core::Game::Initialize(const std::vector<PlayerInit>& players, unsigned int map_size) {
    // Create map with given size
    map_ = world::Map(map_size);
    // Initialize players
    for (const auto& player : players) {
        players_.push_back(std::make_unique<Player>(Player(player.name, player.deck)));
        // Add a capital building to each player
        players_.back()->AddBuilding(std::make_shared<buildings::Building>(buildings::BuildingType::kCapital));
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
    // TODO: Update resources for all players

    // Update alive players
    for (auto it = players_.begin(); it != players_.end(); ) {
        if (!(*it)->IsAlive()) {
            if (debug_) {
                std::cout << (*it)->GetName() << " has lost the game." << std::endl;
            }
            unsigned int deleted_index = std::distance(players_.begin(), it);
            dead_players_.push_back(std::move(*it));
            it = players_.erase(it);
            nof_players_--;
            // Check that current_turn is adjusted correctly
            current_turn_ = current_turn_ >= deleted_index ? 
                current_turn_ - 1 : current_turn_;
            // Secure the validity of current_turn_
            current_turn_ = current_turn_ % nof_players_;
            if (nof_players_ < 2) {
                // Game over
                if (debug_) {
                    std::cout << "Game over! Winner: " << players_[current_turn_]->GetName() << std::endl;
                }
                return;
            }
        } else {
            ++it;
        }
    }
    // NOTE: Keep current_turn_ changes right after player removal above for consistent current_turn_
    // TODO Verify that above is correct (especially edge case current_turn_ == deleted_index)
    turn_++;
    current_turn_ = (current_turn_ + 1) % nof_players_;
    if (debug_) {
        std::cout << "It's now " << players_[current_turn_]->GetName() << "'s turn. " << "(global turn " << turn_ << ")" << std::endl;
    }

    // Draw new hand for the current player (In one round everyone gets to draw a hand once)
    players_[current_turn_]->DrawHand();
}