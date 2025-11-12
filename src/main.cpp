#include <iostream>
#include <assert.h>
#include <string>
#include <fstream>

#include "core/game.hpp"
#include "core/tests.hpp"
#include "ui/user_interface.hpp"


// the main function
int main() {
    
    /*
    // Load game from save file
    core::Game game;
    std::ifstream inFile("saveFile.txt");
    if (inFile.is_open()) {
        game.Load(inFile);
        inFile.close();
        core::DebugGameState(game);
    } else {
        std::cout << "No save file found." << std::endl;
    }
    */

    // Run tests
    core::TestGameInitializationAndTurns();
    core::TestGameSaveAndLoad();

    // init game
    std::vector<core::Game::PlayerInit> players;
    core::Game game;

    // init user interface
    ui::UserInterface user_interface;
    if (!user_interface.Initialize()) {
        return EXIT_FAILURE;
    }

    // Main graphics loop
    while (user_interface.GetWindow().isOpen()) {

        // Handle events
        while (user_interface.PollEvent()) {
            
            // Handle events that do not depend on game state etc. (window resizing and closing)
            user_interface.HandleGeneralEvents();

            // show main menu if game is not initialized yet
            if (!game.IsInitialized()) {
                user_interface.HandleMainMenuEvents();

                // Initialize game on pressing "Play"
                if (user_interface.IsPlayClicked()) {

                    // Get game initialization options from selectors
                    std::vector<int> options = user_interface.GetSelectedOptions();
                    unsigned int player_count = options[0];
                    unsigned int map_size = options[1];

                    // Create players
                    for (unsigned int i = 0; i < player_count; ++i) {
                        players.emplace_back(core::Game::PlayerInit{
                            "Player " + std::to_string(i + 1),
                            cards::Deck() // TODO: add custom starter decks
                        });
                    }

                    // Initializing through main menu testing
                    game.Initialize(players, map_size);
                    user_interface.InitializeMapRenderer(game.GetMap());

                    assert(game.IsInitialized() && !game.IsOver());
                    assert(game.GetCurrentPlayer().GetName() == "Player 1");
                    for (int i = 0; i < 5; i++) {
                        game.NextTurn();
                        assert(game.GetCurrentTurn() == i + 1);
                        // Add resources to the current player for testing
                        game.GetCurrentPlayer().AddResources({core::Resource(core::ResourceType::kGold, 10 + 2 * i)});
                    }
                    // Save game state after initialization
                    std::ofstream outFile("saveFile.txt");
                    if (outFile.is_open()) {
                        game.Save(outFile);
                        outFile.close();
                    }
                }

            // Render map and other game ui elements
            } else {
                // TODO: temporarily keep this to accomodate other ui branches
            }
        }

        user_interface.DrawAndDisplay(game.IsInitialized());
    }
    return 0;
}