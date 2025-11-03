#include <iostream>
#include <assert.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "core/game.hpp"
#include "core/tests.hpp"
#include "ui/selection.hpp"
#include "ui/center_origin.hpp"
#include "ui/map_renderer.hpp"


// the main function
int main(){
    
    // Graphics init
    sf::RenderWindow window(sf::VideoMode(constants::kInitWindowWidth, constants::kInitWindowHeight), "StrategyGame");
    sf::View view = window.getDefaultView();
    sf::Vector2f view_size = view.getSize();

    // Game state
    bool start = false;

    // initialize sprite for background image
    sf::Texture texture;
    if (!texture.loadFromFile("../texture/background.jpg")){
        return EXIT_FAILURE;
    }
    sf::Sprite sprite(texture);

    // Initialize font with shared_ptr
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile("../texture/times.ttf")){
        return EXIT_FAILURE;
    }
    
    // Initialize texts and set their position in the main menu
    sf::Text name("Placeholder", *font, 50);
    name.setPosition(view_size.x*0.1, view_size.y*0.1);

    sf::Text play("Play", *font, 40);
    play.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    sf::Text options("Options", *font, 35);
    play.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    // Initialize option selectors for the main menu
    std::vector<std::pair<std::string, int>> playerCountTexts {
        std::pair("Two players", 2), std::pair("Three players", 3), std::pair("Four players", 4) };
    ui::Selection playerCountSelection(playerCountTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.35));

    std::vector<std::pair<std::string, int>> mapSizeTexts {
        std::pair("Small map", 3), std::pair("Normal map", 5), std::pair("Large map", 7) };
    ui::Selection mapSelection(mapSizeTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.45));

    std::vector<std::pair<std::string, int>> deckTexts {
        std::pair("Deck 1", 1), std::pair("Deck 2", 2), std::pair("Deck 3", 3) };
    ui::Selection deckSelection(deckTexts, font, 35, sf::Vector2f(view_size.x*0.25, view_size.y*0.55));


    // MapRenderer map;

    // Main menu graphics loop
    while (window.isOpen()) {
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Handle events
        sf::Event event;

        while (window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                view.setSize(sf::Vector2f(event.size.width, event.size.height));
                window.setView(view);
            }


            if (!start) {  


            if (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left) {
                playerCountSelection.UpdateState(window);
                mapSelection.UpdateState(window);
                deckSelection.UpdateState(window); 
            }

            // Initialize game on pressing "Play"
            if (event.type == sf::Event::MouseButtonReleased && 
                event.mouseButton.button == sf::Mouse::Left) {
                if(play.getGlobalBounds().contains(mousePos)) {
                    
                    start = true;

                    unsigned int map_size = mapSelection.GetSelectedOption();
                    unsigned int player_count = playerCountSelection.GetSelectedOption();
                    std::vector<core::Game::PlayerInit> players;
                    core::Game game;

                    // Create players
                    for (unsigned int i = 0; i < player_count; ++i) {
                        players.emplace_back(core::Game::PlayerInit{
                            "Player " + std::to_string(i + 1),
                            cards::Deck() // TODO: add custom starter decks
                        });
                    }

                    // Initializing through main menu testing
                    game.Initialize(players, map_size);
                    MapRenderer map(game.GetMap(), 50);
                    while (true) {
                        window.clear();
                        map.DrawTo(window);
                        window.display();
                    }
                    

/*                     assert(game.IsInitialized() && !game.IsOver());
                    assert(game.GetCurrentPlayer().GetName() == "Player 1");
                    for (int i = 0; i < 5; i++) {
                        game.NextTurn();
                        assert(game.GetCurrentTurn() == i + 1);
                    }
                    for (int i = 0; i < player_count - 1; i++) {
                        const std::shared_ptr<buildings::Building> capital = game.GetCurrentPlayer().GetBuildings().front();
                        game.GetCurrentPlayer().RemoveBuilding(capital);
                        game.NextTurn();
                        assert(game.GetNofPlayers() == player_count - (i + 1));
                    }
                    assert(game.IsOver());
                    return 0;  
                 */
                }
            }

        // Make play button slightly larger if mouse is hovering on it
        if(play.getGlobalBounds().contains(mousePos)) {
            play.setScale(1.1,1.1);
        } else {
            play.setScale(1,1);
        }

        playerCountSelection.UpdateHovered(window);
        mapSelection.UpdateHovered(window);
        deckSelection.UpdateHovered(window); 

        // Clear the screen
        window.clear();

        // Draw the sprites and selectors
        window.draw(sprite);
        playerCountSelection.DrawTo(window);
        mapSelection.DrawTo(window);
        deckSelection.DrawTo(window); 
        window.draw(name);
        window.draw(play);

        } else if (start) { 

        }



    }

        // Update the window
        window.display();
    }

    core::TestGameInitializationAndTurns();

    return 0;
}