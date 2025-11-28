#include "core/game.hpp"
#include "cards/card.hpp"
// Tests included here to avoid circular dependency issues in .hpp files
// (Game needs PrintTestMsg, and Tests needs Game)
#include "core/utils.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

void core::Game::Initialize(const std::vector<PlayerInit>& players, unsigned int map_size) {
    // Create map with given size
    map_ = world::Map(map_size, world::Map::GenerationMethod::Stripes);
    // Initialize players
    nof_players_ = players.size();

    // Temp generate capital locations
    spawn_tiles_ = map_.get_n_spawn(nof_players_);

    unsigned int playerNum = 0;
    for (const auto& player : players) {
        players_.push_back(std::make_shared<Player>(Player(player.name, player.deck)));
        
        // Add a capital building to each player
        //switch from create empty to create when want to store the player and tile to building
        players_.back()->AddBuilding(buildings::CapitalBuilding::Create(spawn_tiles_[playerNum], players_.back(), 100));
        players_.back()->SetDeck(player.deck);
        playerNum += 1;
    }
    nof_players_ = players_.size();

    is_initialized_ = true;
    if (debug_) {
        // Use PrintTestMsg for cleaner test message handling
        core::PrintTestMsg("Initialized game with ", nof_players_, " players and map size ", map_size, "x", map_size);
    }
}

void core::Game::Save(std::ostream& file) const{
    time_t timestamp = time(nullptr);
    file << timestamp << "\n";

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
    // Loads timestamp
    std::string timeStr = core::DecodeTimeFromFile(file);
    timeStr.pop_back(); // Remove newline for consistent debug output
    debug_ ? core::PrintTestMsg("Loading game saved at: ", timeStr) : void();

    is_initialized_ = (GetStringFromLine(file) == "1");
    nof_players_ = GetIntFromLine(file);

    // Temporary deck for player initialization.
    std::vector<std::shared_ptr<cards::Card>> empty_cards = {};
    cards::Deck test_deck = cards::Deck(empty_cards, 0U);

    // Load players
    players_.clear();
    for (unsigned int i = 0; i < nof_players_; ++i) {
        Player player(GetStringFromLine(file), test_deck.Clone());
        file >> player;
        players_.push_back(std::make_shared<Player>(std::move(player)));
    }

    size_t dead_players_size = GetIntFromLine(file);

    // Load dead players
    dead_players_.clear();
    for (unsigned int i = 0; i < dead_players_size; ++i) {
        Player player(GetStringFromLine(file), test_deck.Clone());
        file >> player;
        dead_players_.push_back(std::make_shared<Player>(std::move(player)));
    }

    current_turn_ = GetIntFromLine(file);
    turn_ = GetIntFromLine(file);

    // After map has been loaded, use players to add the building to the map
    debug_ ? core::PrintTestMsg("Loaded game with ", nof_players_, " players.") : void();
}

bool core::Game::IsOver() const {
    return nof_players_ == 1;
}
void core::Game::NextTurn() {

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

    auto& player = players_[current_turn_];

    // Draw new hand for the current player (In one round everyone gets to draw a hand once)
    players_[current_turn_]->DrawHand();

    // Update resources for the current player based on their buildings
    for (auto& building : player->GetBuildings()) {
        building->atTurnEnd();
    }
}

bool core::Game::PlayCardOnTile(std::shared_ptr<cards::Card> card, std::shared_ptr<world::Tile> tile) {
    auto& player = players_[current_turn_];
    return player->GetHand()->PlayCard(card, *tile, player);
}