#include "core/game.hpp"
// Tests included here to avoid circular dependency issues in .hpp files
// (Game needs PrintTestMsg, and Tests needs Game)
#include "core/tests.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

void core::Game::Initialize(const std::vector<PlayerInit>& players, unsigned int map_size) {
    // Create map with given size
    map_ = world::Map(map_size);
    // Initialize players
    for (const auto& player : players) {
        players_.push_back(std::make_unique<Player>(Player(player.name, player.deck)));
        // Add a capital building to each player
        players_.back()->AddBuilding(std::make_shared<buildings::CapitalBuilding>());
    }
    nof_players_ = players_.size();
    is_initialized_ = true;
    if (debug_) {
        // Use PrintTestMsg for cleaner test message handling
        core::PrintTestMsg("Initialized game with ", nof_players_, " players and map size ", map_size, "x", map_size);
    }
}

void core::Game::Save(std::ostream& file) const{
    // TODO
    // file << time;
    file << is_initialized_  << "\n";
    file << nof_players_ << "\n";
    for (const auto& player : players_) {
        file << *player;
    }
    file << dead_players_.size() << "\n";
    for (const auto& player : dead_players_) {
        file << *player;
    }
    file << current_turn_ << "\n";
    file << turn_ << "\n";
    // file << map_
}

void core::Game::Load(std::istream& file){
    // TODO
    //std::string time;
    //std::getline(file, time);
    std::string is_initialized_str;
    std::getline(file, is_initialized_str);
    is_initialized_ = (is_initialized_str == "1");

    std::string nof_players_str;
    std::getline(file, nof_players_str);
    nof_players_ = std::stoul(nof_players_str);

    // Load players
    players_.clear();
    for (unsigned int i = 0; i < nof_players_; ++i) {
        std::string player_name_str;
        std::getline(file, player_name_str);
        Player player(player_name_str, cards::Deck());
        file >> player;
        players_.push_back(std::make_unique<Player>(std::move(player)));
    }

    std::string dead_players_size_str;
    std::getline(file, dead_players_size_str);
    size_t dead_players_size = std::stoul(dead_players_size_str);

    // Load dead players
    dead_players_.clear();
    for (unsigned int i = 0; i < dead_players_size; ++i) {
        std::string player_name_str;
        std::getline(file, player_name_str);
        Player player(player_name_str, cards::Deck());
        file >> player;
        dead_players_.push_back(std::make_unique<Player>(std::move(player)));
    }

    std::string current_turn_str;
    std::getline(file, current_turn_str);
    current_turn_ = std::stoul(current_turn_str);

    std::string turn_str;
    std::getline(file, turn_str);
    turn_ = std::stoul(turn_str);

    // After map has been loaded, use players to add the building to the map
    debug_ ? core::PrintTestMsg("Loaded game with ", nof_players_, " players.") : void();
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
                core::PrintTestMsg((*it)->GetName(), " has lost the game.");
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
                    core::PrintTestMsg("Game over! Winner: ", players_[current_turn_]->GetName());
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
        core::PrintTestMsg("It's now ", players_[current_turn_]->GetName(), "'s turn. ", "(global turn ", turn_, ")");
    }

    // Draw new hand for the current player (In one round everyone gets to draw a hand once)
    players_[current_turn_]->DrawHand();
}