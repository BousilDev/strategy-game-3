#include <iostream>
#include <assert.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "core/game.hpp"
#include "ui/selection.hpp"
#include "ui/center_origin.hpp"


// the main function
int main(){
    
    // Graphics init 

    sf::RenderWindow window(sf::VideoMode(800,600), "StrategyGame");
    sf::View view = window.getDefaultView();
    sf::Vector2f view_size = view.getSize();

    sf::Texture texture;
    if (!texture.loadFromFile("../texture/background.jpg")){
        return EXIT_FAILURE;
    }
    sf::Sprite sprite(texture);

    // Using shared ptr for font
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile("../texture/times.ttf")){
        return EXIT_FAILURE;
    }

    
    sf::Text name("Placeholder", *font, 50);
    name.setPosition(view_size.x*0.1, view_size.y*0.1);

    sf::Text play("Play", *font, 40);
    play.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    sf::Text options("Options", *font, 35);
    play.setPosition(sf::Vector2f(view_size.x*0.1, view_size.y*0.2));

    // Initialize options selection
    std::vector<std::pair<sf::Text, int>> playerCountOptions;
    playerCountOptions.emplace_back(sf::Text("One player", *font, 35), 1);
    playerCountOptions.emplace_back(sf::Text("Two players", *font, 35), 2);
    playerCountOptions.emplace_back(sf::Text("Three players", *font, 35), 3);
    playerCountOptions.emplace_back(sf::Text("Four players", *font, 35), 4);
    ui::Selection playerCountSelection(playerCountOptions, sf::Vector2f(200,225));

    std::vector<std::pair<sf::Text, int>> mapSizeOptions;
    mapSizeOptions.emplace_back(sf::Text("Small map", *font, 35), 3);
    mapSizeOptions.emplace_back(sf::Text("Normal map", *font, 35), 5);
    mapSizeOptions.emplace_back(sf::Text("Large Map", *font, 35), 7);
    ui::Selection mapSelection(mapSizeOptions, sf::Vector2f(200,275));

    std::vector<std::pair<sf::Text, int>> deckOptions;
    deckOptions.emplace_back(sf::Text("Deck 1", *font, 35),1);
    deckOptions.emplace_back(sf::Text("Deck 2", *font, 35),2);
    deckOptions.emplace_back(sf::Text("Deck 3", *font, 35),3);
    ui::Selection deckSelection(deckOptions, sf::Vector2f(200,325));

    // Main menu graphics loop
    while (window.isOpen()) {
        
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        sf::Event event;
        while (window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                view.setSize(sf::Vector2f(event.size.width, event.size.height));
                window.setView(view);
            }

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
                    assert(game.IsInitialized() && !game.IsOver());
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
                }
            }
        }

        if(play.getGlobalBounds().contains(mousePos)) {
            play.setScale(1.1,1.1);
        } else {
            play.setScale(1,1);
        }

        playerCountSelection.UpdateHovered(window);
        mapSelection.UpdateHovered(window);
        deckSelection.UpdateHovered(window); 

        window.clear();

        window.draw(sprite);
        playerCountSelection.DrawTo(window);
        mapSelection.DrawTo(window);
        deckSelection.DrawTo(window); 
        window.draw(name);
        window.draw(play);

        window.display();
    }


    // Testing the Game class
    unsigned int map_size = 5;
    core::Game game;
    const std::vector<core::Game::PlayerInit> players = {
        {"Test Gamer", cards::Deck()},
        {"Bob the Builder", cards::Deck()},
        {"Jari the Destroyer", cards::Deck()},
        {"Markku the Conqueror", cards::Deck()}
    };


    game.Initialize(players, map_size);
    assert(game.IsInitialized() && !game.IsOver());
    assert(game.GetCurrentPlayer().GetName() == "Test Gamer");
    for (int i = 0; i < 5; i++) {
        game.NextTurn();
        assert(game.GetCurrentTurn() == i + 1);
    }
    for (int i = 0; i < 3; i++) {
        const std::shared_ptr<buildings::Building> capital = game.GetCurrentPlayer().GetBuildings().front();
        game.GetCurrentPlayer().RemoveBuilding(capital);
        game.NextTurn();
        assert(game.GetNofPlayers() == 4 - (i + 1));
    }
    assert(game.IsOver());
    
    /*
    Use this once game.NextTurn() is fully implemented
    while (!game.IsOver()) {
        game.NextTurn();
    }
    */
    return 0;
}