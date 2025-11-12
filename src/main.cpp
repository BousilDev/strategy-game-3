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

    // Graphics init
    ////sf::RenderWindow window(sf::VideoMode(constants::kInitWindowWidth, constants::kInitWindowHeight), "StrategyGame");
    ////sf::View view = window.getDefaultView();
    ////sf::Vector2f view_size = view.getSize();

    // Game state
    bool start = false;

    // Initialize font with shared_ptr
    ////auto font = std::make_shared<sf::Font>();
    ////if (!font->loadFromFile("./texture/times.ttf")){
    ////    return EXIT_FAILURE;
    ////}
    
    // Main menu init
    ////ui::MainMenu main_menu;
    ////main_menu.Initialize(font, view_size);

    // Map renderer init
    ////ui::MapRenderer map_renderer;

    // init game
    std::vector<core::Game::PlayerInit> players;
    core::Game game;

    // init user interface
    ui::UserInterface user_interface;

    // Main graphics loop
    while (user_interface.GetWindow().isOpen()) {
        
        user_interface.UpdateMousePos();
        ////sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Handle events
        ////sf::Event event;
        while (user_interface.PollEvent()) {
            
            user_interface.HandleGeneralEvents();
            ////if (event.type == sf::Event::Closed)
            ////    window.close();
            ////
            ////if (event.type == sf::Event::Resized) {
            ////    view.setSize(sf::Vector2f(event.size.width, event.size.height));
            ////    window.setView(view);
            ////}
            if (!start) { // main menu
                user_interface.HandleMainMenuEvents();
                ////if (event.type == sf::Event::MouseButtonReleased && 
                ////    event.mouseButton.button == sf::Mouse::Left) {

                //TODO: add a check for clicking stuff???

                    // update elements that do something when LMB is released
                    ////main_menu.UpdateLMBReleased(window); 

                    // Initialize game on pressing "Play"
                    if (user_interface.IsPlayClicked()) {

                        // Things that are done when play is clicked
                        start = true;

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
                //} // end if lmb is clicked



                // Update the elements that do something when hovered over
                ////main_menu.UpdateHovered(window, mousePos);
                // Clear the screen
                ////window.clear();
                // Draw the sprites and selectors
                ////main_menu.DrawTo(window);
            } else if (start) {
                // Map rendering loop
                ////window.clear();
                ////map_renderer.DrawTo(window);
            }
        }
        // Update the window
        ////window.display();
        // Update the window
        user_interface.DrawAndDisplay(start);
    }
    return 0;
}