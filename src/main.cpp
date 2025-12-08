#include <iostream>
#include <assert.h>
#include <string>
#include <fstream>
#include <filesystem>

#include "constants/deck_and_card_options.hpp"
#include "core/game.hpp"
#include "ui/user_interface.hpp"

// the main function
int main() {

    // init game
    std::vector<core::Game::PlayerInit> players;
    core::Game game;

    // init user interface
    ui::UserInterface user_interface;
    if (user_interface.Initialize(game)) {
        return EXIT_FAILURE;
    }

    sf::Clock clock;
    
    // Main graphics loop
    while (user_interface.GetWindow().isOpen()) {

        // Time since last frame
        sf::Time dt = clock.restart();
        float delta_seconds = dt.asSeconds();

        // Handle events
        while (user_interface.PollEvent()) {
            user_interface.HandleEvent(game.IsInitialized(), game.IsEnded());
   
            // main menu if game is not initialized yet
            if (!game.IsInitialized()) {

                // Initialize game on pressing "Start"
                if (user_interface.IsStartClicked()) {

                    // Get game initialization options from selectors
                    unsigned int player_count = user_interface.GetSelectedPlayerCount();
                    unsigned int map_size = user_interface.GetSelectedMapSize();
                    unsigned int deck = user_interface.GetSelectedDeck();
                    std::string game_name = user_interface.GetGameName();
                    std::shared_ptr<cards::Deck> selected_deck;
                    switch (deck) {
                        case 1:
                            selected_deck = card_constants::kStarterDeck.Clone();
                            break;
                        case 2:
                            selected_deck = card_constants::kBalancedDeck.Clone();
                            break;
                        default:
                            selected_deck = card_constants::kAggroDeck.Clone();
                            break;
                    }
                    // Create players
                    for (unsigned int i = 0; i < player_count; ++i) {
                        players.emplace_back(core::Game::PlayerInit{
                            "Player " + std::to_string(i + 1),
                             selected_deck->Clone()
                        });
                    }

                    // Initializing through main menu testing
                    game.Initialize(players, map_size);
                    user_interface.InitializeMapRenderer(game);

                    game.SetName(game_name);
                    players.clear();

                    game.NextTurn();

                    // Handling an event to ready-up map renderer
                    user_interface.HandleEvent(true, game.IsEnded());

                } else if (user_interface.IsLoadClicked()) {
                    // Things that are done when load is clicked
                    std::cout << "Load clicked! Save file path: " << user_interface.GetLastClickedSavePath() << std::endl;
                    // Load game from save file
                    std::ifstream inFile(user_interface.GetLastClickedSavePath());
                    if (inFile.is_open()) {
                        game.Load(inFile);
                        inFile.close();
                        user_interface.InitializeMapRenderer(game);
                    } else {
                        std::cout << "No save file found." << std::endl;
                    }
                }
            } else {
                // TODO: things that are done when the game has been initialized
            }
        }
        // Update window size etc. outside the event handling loop
        user_interface.UpdateOutsideEventLoop(game.IsInitialized(), delta_seconds);
        user_interface.DrawAndDisplay(game.IsInitialized(), *game.IsEnded().get());
    }
    return 0;
}