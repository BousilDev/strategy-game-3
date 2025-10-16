#include <iostream>
#include "core/game.hpp"

// the main function
int main(){
    // Testing the Game class
    unsigned int map_size = 5;
    core::Game game;
    const std::vector<core::Game::PlayerInit> players = {
        {"Test Gamer", cards::Deck()},
        {"Bob the Builder", cards::Deck()}
    };
    game.Initialize(players, map_size);
    for (int i = 0; i < 5; i++) {
        game.NextTurn();
    }
    /*
    Use this once game.NextTurn() is fully implemented
    while (!game.IsOver()) {
        game.NextTurn();
    }
    */
    return 0;
}