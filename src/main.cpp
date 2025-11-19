#include <iostream>
#include <assert.h>
#include <string>
#include <fstream>
#include <filesystem>

#include "core/game.hpp"
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

    bool start = false;

    // init game
    std::vector<core::Game::PlayerInit> players;
    core::Game game;

    // init user interface
    ui::UserInterface user_interface;
    if (user_interface.Initialize(game)) {
        return EXIT_FAILURE;
    }

    // Main graphics loop
    while (user_interface.GetWindow().isOpen()) {

        // Handle events
        while (user_interface.PollEvent()) {
            user_interface.HandleEvent(start);
   
            // main menu if game is not initialized yet
            if (!game.IsInitialized()) {

                // Initialize game on pressing "Start"
                if (user_interface.IsStartClicked()) {

                    // TODO: temp for ui map handling
                    start = true;

                    // Get game initialization options from selectors
                    unsigned int player_count = user_interface.GetSelectedPlayerCount();
                    unsigned int map_size = user_interface.GetSelectedMapSize();
                    unsigned int deck = user_interface.GetSelectedDeck();

                    std::vector<std::shared_ptr<cards::Card>> empty_cards = {};
                    cards::Deck test_deck = cards::Deck(empty_cards, 0U);
                    // Create players
                    for (unsigned int i = 0; i < player_count; ++i) {
                        players.emplace_back(core::Game::PlayerInit{
                            "Player " + std::to_string(i + 1),
                            test_deck.Clone() // TODO: add custom starter decks
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
                } else if (user_interface.IsLoadClicked()) {
                    //TODO: things that are done when load is clicked

                    std::cout << "Load has been clicked!" << std::endl;
                    
                    const std::filesystem::path saves_folder_ = constants::kSavesPath;
                    try {
                        if (!std::filesystem::exists(saves_folder_) || !std::filesystem::is_directory(saves_folder_)) {
                            std::cerr << "Not a directory: " << saves_folder_ << "\n";
                        }
                    
                        for (const auto& entry : std::filesystem::directory_iterator(saves_folder_)) {
                            if (entry.is_regular_file()) {
                                std::cout << entry.path().string() << std::endl;
                            }
                        }
                    } catch (const std::filesystem::filesystem_error& error) {
                        std::cerr << "Filesystem error: " << error.what() << "\n";
                    }
                }
            } else {
                // TODO: things that are done when the game has been initialized
            }
        }
        user_interface.DrawAndDisplay(game.IsInitialized());
    }
    return 0;
}
